#include "UI.h"

//settings
static int fontSize = 24;
static bool darkMode = true;

//windows
static bool testWindow_open = false;
static bool showDemo = false;
static bool showSettings = false;


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
        font_main = io.Fonts->AddFontFromFileTTF("../fonts/Inconsolata-Medium.ttf",fontSize);
        font_bold = io.Fonts->AddFontFromFileTTF("../fonts/Inconsolata-ExtraBold.ttf",fontSize);
        font_light = io.Fonts->AddFontFromFileTTF("../fonts/Inconsolata-Light.ttf",fontSize);

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
            ImGui::GetIO().FontGlobalScale = fontSize / 24.0f;

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            showMainUI();

            if(showDemo) {ImGui::ShowDemoWindow();}
            if(testWindow_open) {showTestWindow();}
            if(showSettings) {showSettingsWindow();}

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

}
