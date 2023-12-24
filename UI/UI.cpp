#include "UI.h"

//settings
static int iMaxFontSize = 42;
static int iFontSize = 32;
static bool bDarkMode = true;
static int iCurrentTheme = 0; //is set to the combo selection
static int iUserWindowWidth = 1600;
static int iUserWindowHeight = 900;
static int iLastStoredFontSize = 32;

//windows
static bool bTestWindow_open = false;
static bool showDemo = false;
static bool showSettings = false;

std::vector<std::string> vFoundThemes;

ImFont *font_main , *font_bold , *font_light;
ImVec4 clearColor = ImVec4(0.45f,0.55f,0.60f,1.00f);
GLFWwindow* mainWindow;

namespace UI {
    int initialize() {
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
            return 1;

        iUserWindowHeight = util::strToInt(config::getProp(config::system,"windowHeight"));
        iUserWindowWidth = util::strToInt(config::getProp(config::system,"windowWidth"));

        //fallback if not found in config
        //config should have values updated to default for second run
        if( iUserWindowHeight == 0) {
        iUserWindowHeight = 480; }
        if( iUserWindowWidth == 0) {
        iUserWindowWidth = 640; }

        //create window
        mainWindow = glfwCreateWindow( iUserWindowWidth,iUserWindowHeight,"Main Window",NULL,NULL);
        if(mainWindow == NULL) { return 1; }
        glfwMakeContextCurrent(mainWindow);
        glfwSwapInterval(1); //vsync

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        font_main = io.Fonts->AddFontFromFileTTF("../fonts/Inconsolata-Medium.ttf",iMaxFontSize );
        font_bold = io.Fonts->AddFontFromFileTTF("../fonts/Inconsolata-ExtraBold.ttf",iMaxFontSize );
        font_light = io.Fonts->AddFontFromFileTTF("../fonts/Inconsolata-Light.ttf",iMaxFontSize );

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(mainWindow,true);
        ImGui_ImplOpenGL3_Init("#version 130");

        vFoundThemes = config::getAllThemeNames();
        iFontSize = util::strToInt(config::getProp(config::system,"fontSize"));
        for(int i = 0;i<vFoundThemes.size();i++){
            if(vFoundThemes[i] == config::getProp(config::system,"theme")){
               iCurrentTheme = i;
            }
        }
        refreshTheme();

        return 0;

    }

    bool mainLoopCondition() {
        return !glfwWindowShouldClose(mainWindow);
    }


    void glfw_error_callback(int error, const char* description) {
        fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    }

    void mainLoop(){
        //poll and handle events (inputs, window resize, etc)
        glfwPollEvents();
        ImGui::GetIO().FontGlobalScale = float(iFontSize) / float(iMaxFontSize);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        showMainUI();

        //windows
        if(showDemo) { ImGui::ShowDemoWindow(); }
        if( bTestWindow_open ) { showTestWindow(); }
        if(showSettings) { showSettingsWindow(); }

        //rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(mainWindow, &display_w,&display_h);
        glViewport(0,0,display_w,display_h);
        glClearColor(clearColor.x * clearColor.w , clearColor.y * clearColor.w , clearColor.z * clearColor.w , clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(mainWindow);
    }

    void cleanUp(){
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
    }

    void showMainUI(){
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        ImGui::Begin("Main",NULL,ImGuiWindowFlags_NoCollapse);
        if(ImGui::Button("Toggle test Window")) { bTestWindow_open = !bTestWindow_open; }
        if(ImGui::Button("Toggle Demo Window")) { showDemo = !showDemo; }
        if(ImGui::Button("Toggle Settings Window")) { showSettings = !showSettings; }
        ImGui::End();
    }

    void showTestWindow(){
        ImGui::Begin("Test Window",NULL);
        ImGui::PushFont(font_light);
        ImGui::Text("I'm a test window!");
        ImGui::PopFont();
        ImGui::PushFont(font_main);
        if(ImGui::Button("Close")) { bTestWindow_open = false; }
        ImGui::PopFont();
        ImGui::End();
    }

    void showSettingsWindow(){
        ImGui::Begin("Settings",NULL);

        if (ImGui::BeginCombo("Theme",vFoundThemes[iCurrentTheme].c_str())){
            //for each theme
            for(int i = 0;i<vFoundThemes.size();i++){
                const bool bSelected = (iCurrentTheme == i);
                if(ImGui::Selectable(vFoundThemes[i].c_str(),bSelected)) {
                    iCurrentTheme = i;
                    //selection change is here
                    config::update(config::system,"theme",vFoundThemes[i]);
                    refreshTheme();
                }
                if( bSelected ) { ImGui::SetItemDefaultFocus(); }
            }
            ImGui::EndCombo();
        }

        if(ImGui::Button("Default")) { iFontSize = 24; };
        ImGui::SameLine();
        ImGui::PushFont(font_bold);
        ImGui::SliderInt("Font Size",&iFontSize,10,iMaxFontSize);
        ImGui::PopFont();
        if(ImGui::Button("Toggle Dark Mode")) {
            bDarkMode = !bDarkMode;
            if( bDarkMode ) { ImGui::StyleColorsDark(); }
            else ImGui::StyleColorsLight();
            config::update(config::theme,"darkMode",std::to_string(bDarkMode));

        } ;
        ImGui::End();
    }

    void setThemeByPath(const char* fileLocation) {
        std::string sLine, sBuild, sProp;
        std::ifstream fileIn;
        fileIn.open(fileLocation);

        if( fileIn.is_open()){

            //for each line
            while(getline( fileIn,sLine )) {
                sProp.clear();
                sBuild.clear();

                for(char element : sLine ) {
                    if(element == '='){
                        sProp = sBuild;
                        sBuild.clear();
                    } else {
                        sBuild.push_back(element);
                    }

                }
                config::update(config::theme,sProp.c_str(),sBuild);
            }
        } else {
            util::qPrint("File",fileLocation,"was unable to be opened!");
        }

        fileIn.close();

        //theme options
        iFontSize = util::strToInt(config::getProp(config::system,"fontSize"));
        bDarkMode = util::strToInt(config::getProp(config::theme,"darkMode"));


        //apply any manual settings

    }

    void saveThemeToFile(const char* name) {
        std::ofstream fileOut;
        std::string sFileName;
        sFileName.append("../UI/Themes/");
        sFileName.append(name);
        sFileName.append(".hbtheme");

        fileOut.open( sFileName );

        if( fileOut.is_open()){
            for(auto option : config::getConfig(config::theme)){
                fileOut << option.first;
                fileOut << '=';
                fileOut << option.second;
                fileOut << '\n';
            }

            util::qPrint("File",sFileName,"was saved!");

        } else {
            util::qPrint("File",sFileName,"could not be saved!");
        }

        fileOut.close();


    }

    void setAllThemeNames(){
        //for each file in theme path
        for(auto& file : std::filesystem::directory_iterator("../UI/Themes/")){
            if(file.is_regular_file() && (file.path().extension() == ".hbtheme")){
                vFoundThemes.push_back(getThemeNameByPath(file.path()));
            }
        }
        assignCurrentThemeValueByName(config::getProp(config::system,"theme"));

    }

    std::string getThemeNameByPath(std::string sThemePath ) {
        bool bDotFound = false;
        std::string sBuild;

        //reverse for loop
        for(int i = sThemePath.length();i>1;i--){
            if( sThemePath[i] == '.'){
                bDotFound = true;
                continue;
            }
            if( sThemePath[i] == '/') { return sBuild; }
            if( bDotFound ) { sBuild.insert( sBuild.begin(),sThemePath[i]); }
        }

        return std::string("Error :(");
    }

    std::string getThemePathByName(std::string sName ) {
        std::string sOutput;
        if(!util::searchVector(vFoundThemes,sName )) {
            sName = "Default";
            config::update(config::system,"theme",sName );
            assignCurrentThemeValueByName("Default");
        }
        sOutput.append("../UI/Themes/");
        sOutput.append( sName );
        sOutput.append(".hbtheme");
        return sOutput;
    }


    void setThemeByName(std::string name, bool updateConfig) {
        setThemeByPath(getThemePathByName(name).c_str());
        if(updateConfig) { config::update(config::system,"theme",vFoundThemes[iCurrentTheme]); }
    }

    void assignCurrentThemeValueByName ( std::string name ) {
        for(int i = 0;i<vFoundThemes.size();i++){
            if( vFoundThemes[i] == name){
                iCurrentTheme = i;
                break;
            }
        }
    }

    //called each second in main tick_sec function
    void tick_sec() {
        //update font config
        if(iLastStoredFontSize != iFontSize){
            iLastStoredFontSize = iFontSize;
            config::update(config::system,"fontSize",std::to_string(iFontSize));
        }
    }

    void refreshTheme() {
        bDarkMode = util::strToInt(config::getProp(config::theme,"darkMode"));
        bDarkMode ? ImGui::StyleColorsDark() : ImGui::StyleColorsLight();
        util::qPrint(bDarkMode);
        util::qPrint(util::strToInt(config::getProp(config::theme,"darkMode")));
        util::qPrint(config::getProp(config::system,"theme"));
    }


}
