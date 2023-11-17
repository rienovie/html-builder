#include <stdio.h>
#include "UI/imgui.h"
#include "UI/imgui_stdlib.h"
#include "UI/imconfig.h"
#include "UI/backends/imgui_impl_glfw.h"
#include "UI/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>


int main () {

    //create window
    GLFWwindow* mainWindow = glfwCreateWindow(1280,720,"Main Window",nullptr,nullptr);
    if(mainWindow == nullptr) { return 1; }
    glfwMakeContextCurrent(mainWindow);
    glfwSwapInterval(1); //vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(mainWindow,true);
    ImGui_ImplOpenGL3_Init("#version 130");

    ImVec4 clearColor = ImVec4(0.45f,0.55f,0.60f,1.00f);

    //main loop
    while(!glfwWindowShouldClose(mainWindow)){

        //poll and handle events (inputs, window resize, etc)
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

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

    //clean up
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(mainWindow);
    glfwTerminate();

    return 0;

}

void initialize() {


}
