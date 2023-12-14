#include "UI.h"

//settings
static int maxFontSize = 42;
static int fontSize = 32;
static bool darkMode = true;

//windows
static bool testWindow_open = false;
static bool showDemo = false;
static bool showSettings = false;

std::map<std::string, std::string> themeMap {
{"fontSize","32"},
{"darkMode","1"}
};


ImFont *font_main , *font_bold , *font_light;
ImVec4 clearColor = ImVec4(0.45f,0.55f,0.60f,1.00f);
GLFWwindow* mainWindow;

namespace UI {
    int initialize() {
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
            return 1;

        const char* glsl_version = "#version 130";

        //create window
        mainWindow = glfwCreateWindow(1600,900,"Main Window",NULL,NULL);
        if(mainWindow == nullptr) { return 1; }
        glfwMakeContextCurrent(mainWindow);
        glfwSwapInterval(1); //vsync

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        font_main = io.Fonts->AddFontFromFileTTF("../fonts/Inconsolata-Medium.ttf",maxFontSize);
        font_bold = io.Fonts->AddFontFromFileTTF("../fonts/Inconsolata-ExtraBold.ttf",maxFontSize);
        font_light = io.Fonts->AddFontFromFileTTF("../fonts/Inconsolata-Light.ttf",maxFontSize);

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(mainWindow,true);
        ImGui_ImplOpenGL3_Init(glsl_version);

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
        ImGui::GetIO().FontGlobalScale = float(fontSize) / float(maxFontSize);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        showMainUI();

        //windows
        if(showDemo) { ImGui::ShowDemoWindow(); }
        if(testWindow_open) { showTestWindow(); }
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
        if(ImGui::Button("Toggle test Window")) testWindow_open = !testWindow_open;
        if(ImGui::Button("Toggle Demo Window")) showDemo = !showDemo;
        if(ImGui::Button("Toggle Settings Window")) showSettings = !showSettings;
        ImGui::End();
    }

    void showTestWindow(){
        ImGui::Begin("Test Window",NULL);
        ImGui::PushFont(font_light);
        ImGui::Text("I'm a test window!");
        ImGui::PopFont();
        ImGui::PushFont(font_main);
        if(ImGui::Button("Close")) { testWindow_open = false; }
        ImGui::PopFont();
        ImGui::End();
    }

    void showSettingsWindow(){
        ImGui::Begin("Settings",NULL);
        if(ImGui::Button("Load From File")) {setThemeByFile("../UI/Themes/default.hbtheme");}
        ImGui::SameLine();
        if(ImGui::Button("Save To File")) {saveThemeToFile("../UI/Themes/","Test");}
        if(ImGui::Button("Default")) { fontSize = 24; };
        ImGui::SameLine();
        ImGui::PushFont(font_bold);
        ImGui::SliderInt("Font Size",&fontSize,8,42);
        ImGui::PopFont();
        if(ImGui::Button("Toggle Dark Mode")) {
            darkMode = !darkMode;
            if(darkMode) { ImGui::StyleColorsDark(); }
            else ImGui::StyleColorsLight();

        } ;
        ImGui::End();
    }

    void setThemeByFile(const char* fileLocation) {
        std::string line, buildStr, property;
        std::ifstream file;
        file.open(fileLocation);

        if(file.is_open()){

            //for each line
            while(getline(file,line)){
                property.clear();
                buildStr.clear();

                for(char element : line){
                    if(element == '='){
                        property = buildStr;
                        buildStr.clear();
                    } else{
                        buildStr.push_back(element);
                    }
                }
                themeMap[property] = buildStr;
                util::qPrint(themeMap[property]);
            }
        } else {
            util::qPrint("File",fileLocation,"was unable to be opened!");
        }

        file.close();

        //theme options
        fontSize = util::strToInt(themeMap["fontSize"]);
        darkMode = util::strToInt(themeMap["darkMode"]);


        //apply any manual settings
        darkMode ? ImGui::StyleColorsDark() : ImGui::StyleColorsLight();

    }

    void saveThemeToFile(const char* themeDir, const char* name) {
        std::ofstream file;
        std::string fileName, property, value, line;
        fileName.append(themeDir);
        fileName.append(name);
        fileName.append(".hbtheme");

        //set map to current values
        themeMap["fontSize"] = std::to_string(fontSize);
        themeMap["darkMode"] = std::to_string(darkMode);

        file.open(fileName);

        if(file.is_open()){
            for(auto option : themeMap){
                file << option.first;
                file << '=';
                file << option.second;
                file << '\n';
            }

            util::qPrint("File",fileName,"was saved!");

        } else {
            util::qPrint("File",fileName,"could not be saved!");
        }

        file.close();


    }



}
