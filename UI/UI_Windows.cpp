#include "UI_Windows.h"

std::map<std::string,bool> win::mWindowBools {
    {"ImGui Demo",false},
    {"Settings",false},
    {"Test",false},
    {"File Browser",false},
    {"Current Hierarchy",false},
    {"Selected Element", false}
};

void win::wMainMenu() {
    ImGui::BeginMainMenuBar();

    if(ImGui::BeginMenu("File")) {
        if(ImGui::MenuItem("Exit")) {
            UI::exitApplication();
        }

        ImGui::EndMenu();
    }

    ImGui::MenuItem("Settings",NULL,&mWindowBools["Settings"]);

    if(ImGui::BeginMenu("Windows")) {
        for(auto& item : mWindowBools) {
            ImGui::MenuItem(item.first.c_str(),NULL,&mWindowBools[item.first]);
        }

        ImGui::EndMenu();
    }

    ImGui::Text(" |  %.3f ms/frame (%.1f FPS)", 1000.0f / UI::ioPtr->Framerate, UI::ioPtr->Framerate);

    ImGui::EndMainMenuBar();
}

void win::mainLoop() {
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    wMainMenu();
    if(mWindowBools["ImGui Demo"]) { ImGui::ShowDemoWindow(); }
    if(mWindowBools["Settings"]) { wSettings(); }
    if(mWindowBools["Test"]) { wTest(); }
    if(mWindowBools["File Browser"]) { wFileBrowser(); }
    if(mWindowBools["Current Hierarchy"]) { wHierarchy(); }
    if(mWindowBools["Selected Element"]) { wSelectedElement(); }

    for(auto& file : html::vLoadedHTMLs) {
        wFileRoot(file);
    }
}

void win::wFileRoot ( html::file* filePTR ) {
    if(mWindowBools.find(filePTR->sFileName) != mWindowBools.end()
    && mWindowBools[filePTR->sFileName] == false) {
        mWindowBools.erase(filePTR->sFileName);
        html::closeFile(filePTR->sFileLocation);
        UI::selectedElement = NULL;
        return;
    }
    mWindowBools[filePTR->sFileName] = true;
    mWindowBools["Current Hierarchy"] = true;

    ImGui::Begin(filePTR->sFileName.c_str(),&mWindowBools[filePTR->sFileName],ImGuiWindowFlags_NoCollapse);

    //raw text
    for(auto& sLine : filePTR->vFileLines) {
        ImGui::Text( "%s", sLine.c_str());
    }

    ImGui::End();
}

void win::wFileBrowser() {
    static std::string sCurrentDir = "/";
    static std::vector<std::string> vCurrentDirSplit;
    static std::vector<std::filesystem::path> vAllCurrentDir;
    static bool bShouldUpdateDir = true;

    if(bShouldUpdateDir) {
        bShouldUpdateDir = false;
        vAllCurrentDir.clear();
        for(auto& item : std::filesystem::directory_iterator {sCurrentDir} ) {
            if(item.path().filename().c_str()[0] == '.' //check if hidden
                || (item.is_regular_file() && item.path().extension() != ".html")
            )  {
                continue;
            }
            vAllCurrentDir.push_back(item.path());
        }
        vCurrentDirSplit.clear();
        vCurrentDirSplit = util::splitStringOnChar(sCurrentDir,'/');
    }

    ImGui::Begin("File Browser",&mWindowBools["File Browser"],ImGuiWindowFlags_NoCollapse);

    ImGui::SeparatorText("Favorites");
    for(auto& sItem : UI::vFavorites) {

        ImGui::PushID(sItem.c_str());
        if(ImGui::Button("X")) {
            config::modifyFavorites(sItem,true);
            UI::bFavoritesUpdated = true;
        }
        BasicToolTip("Remove from favorites");
        ImGui::PopID();

        ImGui::SameLine();
        if(ImGui::Button(sItem.c_str())) {
            if(std::filesystem::is_regular_file(sItem)) {
                html::loadFile(sItem);
                mWindowBools["File Browser"] = false;
            } else {
                sCurrentDir = sItem;
                bShouldUpdateDir = true;
            }
        }
    }

    ImGui::SeparatorText("Current Directory");

    ImGui::BeginChild("CurDir",ImVec2(0,0),ImGuiChildFlags_AutoResizeY,ImGuiWindowFlags_AlwaysHorizontalScrollbar);
    if(ImGui::IsWindowHovered() && ImGui::GetScrollX()) {

    }
    if(ImGui::Button("/")) {
        sCurrentDir = "/";
        bShouldUpdateDir = true;
    }

    //Path buttons
    for(int i = 0; i < vCurrentDirSplit.size(); i++) {
        if(vCurrentDirSplit[i] == "") { continue; }
        ImGui::SameLine();
        if(ImGui::Button(vCurrentDirSplit[i].c_str())) {
            std::string sBuildPath = "";

            //append each path piece prior to build full path
            for(int j = 0; j < i + 1; j++) {
                sBuildPath.append("/");
                sBuildPath.append(vCurrentDirSplit[j]);
            }

            sCurrentDir = sBuildPath;
            bShouldUpdateDir = true;
        }
        ImGui::SameLine();
        ImGui::TextDisabled("/");
    }

    ImGui::EndChild();

    //for each in current directory buttons
    ImGui::BeginChild(sCurrentDir.c_str());
    for(auto& pItem : vAllCurrentDir) {
        if(!util::hasPathPermission(pItem)) { continue; }

        std::string sButtonTxt = pItem.filename();

        //this is needed so ImGui creates functional btns for each fav btn
        ImGui::PushID(&pItem);

        if(std::filesystem::is_directory(pItem)) { sButtonTxt.append("/"); }
        if(ImGui::Button("*")) {
            util::qPrint("New Fav button hit",pItem,sButtonTxt);
            config::modifyFavorites(pItem);
            UI::bFavoritesUpdated = true;
        }
        BasicToolTip("Add to favorites");
        ImGui::PopID();

        ImGui::SameLine();
        if(ImGui::Button(sButtonTxt.c_str())) {
            if(!std::filesystem::is_directory(pItem)) {
                html::loadFile(pItem);
                mWindowBools["File Browser"] = false;
                continue;
            }
            sCurrentDir = pItem;
            bShouldUpdateDir = true;
        }

    }
    ImGui::EndChild();

    ImGui::End();
}

void win::wTest() {

    ImGui::Begin("Test",&mWindowBools["Test"],ImGuiWindowFlags_NoCollapse);

    static std::string sFileLocationInput;
    ImGui::InputTextWithHint("Location","File location",&sFileLocationInput);
    if(ImGui::Button("Load File")) {
        html::loadFile(sFileLocationInput);
    }

    for(auto& item : html::vLoadedHTMLs) {
        ImGui::Text( "%s", item->sFileLocation.c_str());
    }

    if(ImGui::Button("Print Stuff")) {
        for(auto& i : html::vLoadedHTMLs[0]->rootElementPtr->vChildrenPtrs) {
            util::qPrint(i->sElementName);
        }
    }

    ImGui::End();
}

void win::wSettings() {
    ImGui::Begin("Settings",&mWindowBools["Settings"],ImGuiWindowFlags_NoCollapse);

    ImGui::SeparatorText("System");
    for(auto& item : html::vLoadedHTMLs) {
        ImGui::Text( "%s", item->sFileLocation.c_str());
    }
    if(ImGui::Button("Default")) { UI::iFontSize = 20; };
    ImGui::SameLine();
    ImGui::PushFont(UI::font_bold);
    ImGui::SliderInt("Font Size",&UI::iFontSize,UI::limitFontSize.x,UI::limitFontSize.y);
    ImGui::PopFont();

    /* using this to write to the config file, will probably clean up later
     * Put the ImGui color copy in the demo window here and uncomment this code
     *
    if(ImGui::Button("Apply Color Code")) {
        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        //etc...

        auto mTheColVal = config::getAllThemeColorValues();
        for(int i = 0; i < ImGuiCol_COUNT;i++) {
            std::string sBuild = "~";
            sBuild.append(ImGui::GetStyleColorName(i));

            try {
                mTheColVal[sBuild]; //try
                config::update(config::theme,sBuild.c_str(),UI::getStringFromVec4(UI::uiStylePtr->Colors[i]));
            } catch (const std::out_of_range& e) {
                util::qPrint(sBuild,"prop not found!");
                continue;
            }
        }
    } */

    ImGui::SeparatorText("Theme");

    ImGui::Text("Create New Theme Based on Current");
    static char cBufNameInput[32] = "";
    ImGui::InputTextWithHint("New Theme","New Theme Name",cBufNameInput,32,ImGuiInputTextFlags_CallbackCharFilter,themeNameCallback);

    if(cBufNameInput[0] != '\0') {
        static bool bFileDoesNotExist;
        static const char* error;
        static std::string sBuild;

        bFileDoesNotExist = true;
        sBuild.clear();
        error = "";

        //buildString
        for(int i = 0; i < 32 ; i++) {
            if(cBufNameInput[i] == '\0') { break; }
            else sBuild.push_back(cBufNameInput[i]);
        }

        if(util::searchVector(UI::vFoundThemes,sBuild)) {
            bFileDoesNotExist = false;
            error = "Cannot save! Theme already exists!";
        }

        if(bFileDoesNotExist) {
            if(ImGui::Button("Save Theme")) {
                UI::createNewThemeAndSetCurrent(sBuild);

                //I'm honestly not sure if this is the best way but it seems to works so meh
                cBufNameInput[0] = '\0';
            }
        } else ImGui::Text( "%s", error);

    }

    ImGui::NewLine();

    //TODO replace with an icon later
    if(ImGui::Button("Refresh")) {
        config::findAllThemes();
        UI::iCurrentTheme = 0; //makes sure will not be larger than max count
        //check if current theme file no longer exists
        if(!util::searchVector(config::vFoundThemes,config::getProp(config::system,"theme"))) {
            //fallback to Default theme
            config::update(config::system,"theme","Default");
        }
        UI::vFoundThemes = config::getAllThemeNames();
        UI::assignCurrentThemeValueByName(config::getProp(config::system,"theme"));
    } BasicToolTip("Refresh Theme List");

    ImGui::SameLine();
    if (ImGui::BeginCombo("Current",UI::vFoundThemes[UI::iCurrentTheme].c_str())){
        //for each theme
        for(int i = 0;i<UI::vFoundThemes.size();i++){
            const bool bSelected = (UI::iCurrentTheme == i);
            if(ImGui::Selectable(UI::vFoundThemes[i].c_str(),bSelected)) {
                UI::iCurrentTheme = i;
                //selection change is here
                config::update(config::system,"theme",UI::vFoundThemes[i]);
                UI::refreshTheme();
                UI::bDefaultThemeActive = (UI::vFoundThemes[i] == "Default");
            }
            if( bSelected ) { ImGui::SetItemDefaultFocus(); }
        }
        ImGui::EndCombo();
    }

    //ImGui::NewLine();
    ImGui::Indent();
    if(UI::bDefaultThemeActive && false) {
        ImGui::BeginDisabled();
        ImGui::Text("Cannot modify Default theme");
    }
    swThemeOptions();
    swThemeColors();
    if(UI::bDefaultThemeActive && false) {
        ImGui::EndDisabled();
    }
    ImGui::Unindent();

    ImGui::End();
}


void win::swCircleTessTT(bool show_samples) {
    if (show_samples)
        ImGui::SetNextWindowPos(ImGui::GetCursorScreenPos());
    if (show_samples && ImGui::BeginTooltip())
    {
        ImGui::TextUnformatted("(R = radius, N = number of segments)");
        ImGui::Spacing();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        const float min_widget_width = ImGui::CalcTextSize("N: MMM\nR: MMM").x;
        for (int n = 0; n < 8; n++)
        {
            const float RAD_MIN = 5.0f;
            const float RAD_MAX = 70.0f;
            const float rad = RAD_MIN + (RAD_MAX - RAD_MIN) * (float)n / (8.0f - 1.0f);

            ImGui::BeginGroup();

            ImGui::Text("R: %.f\nN: %d", rad, draw_list->_CalcCircleAutoSegmentCount(rad));

            const float canvas_width = IM_MAX(min_widget_width, rad * 2.0f);
            const float offset_x     = floorf(canvas_width * 0.5f);
            const float offset_y     = floorf(RAD_MAX);

            const ImVec2 p1 = ImGui::GetCursorScreenPos();
            draw_list->AddCircle(ImVec2(p1.x + offset_x, p1.y + offset_y), rad, ImGui::GetColorU32(ImGuiCol_Text));
            ImGui::Dummy(ImVec2(canvas_width, RAD_MAX * 2));

            /*
             *       const ImVec2 p2 = ImGui::GetCursorScreenPos();
             *       draw_list->AddCircleFilled(ImVec2(p2.x + offset_x, p2.y + offset_y), rad, ImGui::GetColorU32(ImGuiCol_Text));
             *       ImGui::Dummy(ImVec2(canvas_width, RAD_MAX * 2));
             */

            ImGui::EndGroup();
            ImGui::SameLine();
        }
        ImGui::EndTooltip();
    }
}

void win::swThemeOptions() {
    ImGui::SeparatorText("Options");

    if(ImGui::SliderFloat("Frame Rounding",
                        &UI::uiStylePtr->FrameRounding,
                        UI::limitFrameRounding.x,
                        UI::limitFrameRounding.y,
                        "%.0f")) {
        //grab & popup = same as frame
        UI::uiStylePtr->GrabRounding = UI::uiStylePtr->FrameRounding;
        UI::uiStylePtr->PopupRounding = UI::uiStylePtr->FrameRounding;
        config::update(config::theme,"frameRounding",std::to_string(UI::uiStylePtr->FrameRounding));
    }

    if(ImGui::SliderFloat("Frame Border",
                        &UI::uiStylePtr->FrameBorderSize,
                        UI::limitFrameBorder.x,
                        UI::limitFrameBorder.y,
                        "%.0f")) {
        //popup same as frame
        UI::uiStylePtr->PopupBorderSize = UI::uiStylePtr->FrameBorderSize;
        config::update(config::theme,"frameBorder",std::to_string(UI::uiStylePtr->FrameBorderSize));
    }

    if(ImGui::SliderFloat2("Frame Padding",
                        (float*)&UI::uiStylePtr->FramePadding,
                        UI::limitFramePad.x,
                        UI::limitFramePad.y,
                        "%.0f")) {
        config::update(config::theme,"framePad",UI::getStringFromVec2(UI::uiStylePtr->FramePadding));
    }
    if(ImGui::SliderFloat2("Spacing",
                        (float*)&UI::uiStylePtr->ItemSpacing,
                        UI::limitItemSpacing.x,
                        UI::limitItemSpacing.y,
                        "%.0f")) {
        //inner same
        UI::uiStylePtr->ItemInnerSpacing = UI::uiStylePtr->ItemSpacing;
        config::update(config::theme,"itemSpacing",UI::getStringFromVec2(UI::uiStylePtr->ItemSpacing));
    }

    if(ImGui::SliderFloat("Separator Thickness",
                        &UI::uiStylePtr->SeparatorTextBorderSize,
                        UI::limitSeparatorThickness.x,
                        UI::limitSeparatorThickness.y,
                        "%.1f")) {
        config::update(config::theme,"separatorThickness",std::to_string(UI::uiStylePtr->SeparatorTextBorderSize));
    }

    if(ImGui::SliderFloat("Minimum Grab Size",
                        &UI::uiStylePtr->GrabMinSize,
                        UI::limitGrabMinSize.x,
                        UI::limitGrabMinSize.y,
                        "%.0f")) {
        config::update(config::theme,"grabMinSize",std::to_string(UI::uiStylePtr->GrabMinSize));
    }

    if(ImGui::SliderFloat("Tab Rounding",
                        &UI::uiStylePtr->TabRounding,
                        UI::limitTabRounding.x,
                        UI::limitTabRounding.y,
                        "%.0f")) {
        config::update(config::theme,"tabRounding",std::to_string(UI::uiStylePtr->TabRounding));
    }

    if(ImGui::SliderFloat("Scrollbar Size",
                        &UI::uiStylePtr->ScrollbarSize,
                        UI::limitScrollBarSize.x,
                        UI::limitScrollBarSize.y,
                        "%.0f")) {
        config::update(config::theme,"scrollBarSize",std::to_string(UI::uiStylePtr->ScrollbarSize));
    }

    if(ImGui::SliderFloat("Scrollbar Rounding",
                        &UI::uiStylePtr->ScrollbarRounding,
                        UI::limitScrollBarRounding.x,
                        UI::limitScrollBarRounding.y,
                        "%.0f")) {
        config::update(config::theme,"scrollBarRounding",std::to_string(UI::uiStylePtr->ScrollbarRounding));
    }

    if(ImGui::SliderFloat("Circle Tess",
                        &UI::uiStylePtr->CircleTessellationMaxError,
                        UI::limitCircleTess.x,
                        UI::limitCircleTess.y,
                        "%.2f",
                        ImGuiSliderFlags_AlwaysClamp)) {
        config::update(config::theme,"circleTess",std::to_string(UI::uiStylePtr->CircleTessellationMaxError));
    } swCircleTessTT(ImGui::IsItemActive());

}

void win::swThemeColors() {
    ImGui::SeparatorText("Colors");

    ImVec4 col = UI::uiStylePtr->Colors[ImGuiCol_WindowBg];
    float fUse, fThrowaway;
    ImGui::ColorConvertRGBtoHSV(col.x,col.y,col.z,fUse,fThrowaway,fThrowaway);
    int iHue = util::lerpInt(0,255,fUse);

    if(ImGui::SliderInt("Hue",&iHue,0,255)) {
        //TODO currently working here
    }

    ImGui::NewLine();

    auto mColors = config::getAllThemeColorValues();
    int i = 0;
    for(auto& item : mColors) {
        i = UI::getColorEnum(item.first);
        if(ImGui::ColorEdit4(ImGui::GetStyleColorName(i)
                            ,(float*)&UI::uiStylePtr->Colors[i]
                            ,ImGuiColorEditFlags_DisplayHSV
                            | ImGuiColorEditFlags_PickerHueWheel)) {
            config::update(config::theme
                            ,item.first.c_str()
                            ,UI::getStringFromVec4(UI::uiStylePtr->Colors[i]));
            if(item.first == "~WindowBg") {
                ImVec4 modColor = UI::uiStylePtr->Colors[ImGuiCol_WindowBg];
                float modVal = float(0.5);
                modColor.x *= modVal;
                modColor.y *= modVal;
                modColor.z *= modVal;
                modColor.w = 1;
                UI::uiStylePtr->Colors[ImGuiCol_DockingEmptyBg] = modColor;
            };
        }
    }

}

int win::themeNameCallback ( ImGuiInputTextCallbackData* data ) {
    static char c = data->EventChar;
    return !((isalnum(c) || c == '_' || c == '-' || c == ' '));
    //will return 0 if valid and 1 if not
}

void win::BasicToolTip ( const char* toolTipText ) {
    if(ImGui::IsItemActive() || ImGui::IsItemHovered()) {
        ImGui::SetTooltip("%s",toolTipText);
    }
}

void win::wHierarchy() {
    if(html::vLoadedHTMLs.size() == 0) {
        mWindowBools["Current Hierarchy"] = false;
        return;
    }

    std::string sTitle = html::vLoadedHTMLs[0]->sFileName;
    sTitle.append(" (Hierarchy)");

    ImGui::Begin(sTitle.c_str(),&mWindowBools["Current Hierarchy"]);

    hierarchyPopulate(html::vLoadedHTMLs[0]->rootElementPtr);

    ImGui::End();
}

void win::hierarchyPopulate(html::element* element) {
    for(auto& child : element->vChildrenPtrs) {
        bool bDisable = false; //so EndDisable only runs when beginDisable runs

        ImGui::PushID(child);
        const char* cStrName;
        if(child->vChildrenPtrs.size() != 0) {
            cStrName = child->sElementName.c_str();
        } else {
            cStrName = child->sRawLine.c_str();
        }

        if(UI::selectedElement == child) {
            ImGui::BeginDisabled();
            bDisable = true;
        }
        if(ImGui::Button(cStrName)) {
            UI::selectedElement = child;
            mWindowBools["Selected Element"] = true;
        }
        if(UI::selectedElement == child && bDisable) {
            ImGui::EndDisabled();
            bDisable = false;
        }

        if(child->vChildrenPtrs.size() != 0) {
            ImGui::SameLine();

            //if only single child with no next children
            if(child->vChildrenPtrs.size() == 1
            && child->vChildrenPtrs[0]->vChildrenPtrs.size() == 0
            ) {
                hierarchyPopulate(child);
            } else if(ImGui::TreeNode(cStrName,"Children")) {
                hierarchyPopulate(child);
                ImGui::TreePop();
            }
        }

        ImGui::PopID();

    }
}

void win::wSelectedElement() {
    if(UI::selectedElement == NULL
    ||html::vLoadedHTMLs.size() == 0 ) {
        mWindowBools["Selected Element"] = false;
        return;
    }

    html::element* SE = UI::selectedElement;

    ImGui::Begin("Selected Element",&mWindowBools["Selected Element"]);

    ImGui::Text("File: ");
    ImGui::SameLine();
    ImGui::Text( "%s", SE->filePtr->sFileName.c_str());

    ImGui::Text("Parent: ");
    ImGui::SameLine();
    ImGui::Text("%s", SE->parentPtr->sElementName.c_str());

    ImGui::BeginDisabled();
    ImGui::Checkbox("bIsElement?",&SE->bIsElement);
    ImGui::EndDisabled();

    ImGui::Text("Element: ");
    ImGui::SameLine();
    ImGui::Text( "%s", SE->sElementName.c_str());

    ImGui::Text("Attributes: ");
    ImGui::Indent();
    for(auto& item : SE->mAttributes) {
        ImGui::Text( "%s", item.first.c_str());
        ImGui::SameLine();
        ImGui::Text("=");
        ImGui::SameLine();
        ImGui::Text( "%s", item.second.c_str());
    }
    ImGui::Unindent();

    ImGui::Text("Children: ");
    ImGui::Indent();
    for(auto& item : SE->vChildrenPtrs) {
        ImGui::Text( "%s", item->sElementName.c_str());
    }
    ImGui::Unindent();

    ImGui::Text("Raw: ");
    ImGui::Indent();
    ImGui::BeginChild(SE->sElementName.c_str(),ImVec2(0,0),true);
    ImGui::Text( "%s", SE->sRawLine.c_str());
    ImGui::EndChild();
    ImGui::Unindent();

    ImGui::End();
}

