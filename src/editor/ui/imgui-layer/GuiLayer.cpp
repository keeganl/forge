//
// Created by keega on 6/5/2021.
//

#include "GuiLayer.h"

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void GuiLayer::createContext(GLFWwindow *window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
//    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
//    io.ConfigFlags |= ImGuiConfigFlags_View[]

    io.ConfigFlags |= ImGuiWindowFlags_MenuBar;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.


    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    io.Fonts->AddFontFromFileTTF(R"(C:\Users\keega\Desktop\school\grad\research\forge\assets\fonts\fira-sans\FiraSans-Regular.ttf)", 11.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Setup Dear ImGui style
//    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    colors[ImGuiCol_Text]                   = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
    colors[ImGuiCol_Border]                 = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.204f,0.561f,0.921f, 1.000f);
    colors[ImGuiCol_CheckMark]              = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.2039f,0.561f,0.921f, 1.000f);
    colors[ImGuiCol_Button]                 = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
    colors[ImGuiCol_Header]                 = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
    colors[ImGuiCol_Separator]              = colors[ImGuiCol_Border];
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.204f, 0.561f,0.921f, 1.000f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.204f, 0.561f,0.921f, 1.000f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
    colors[ImGuiCol_DockingPreview]         = ImVec4(0.204f, 0.561f,0.921f, 0.781f);
    colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
    colors[ImGuiCol_PlotLines]              = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.204f, 0.561f,0.921f, 1.000f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(0.204f, 0.561f,0.921f, 1.000f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(0.204f, 0.561f,0.921f, 1.000f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(0.204f, 0.561f,0.921f, 1.000f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(0.204f, 0.561f,0.921f, 1.000f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
    // orange: 0.2039f, 0.561f,  0.921f
    // blue: 0.2039f, 0.561f,  0.921f


    style->ChildRounding = 4.0f;
    style->FrameBorderSize = 1.0f;
    style->FrameRounding = 2.0f;
    style->GrabMinSize = 7.0f;
    style->PopupRounding = 2.0f;
    style->ScrollbarRounding = 12.0f;
    style->ScrollbarSize = 13.0f;
    style->TabBorderSize = 1.0f;
    style->TabRounding = 0.0f;
    style->WindowRounding = 4.0f;

}

void GuiLayer::cleanup() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


void GuiLayer::startFrame()
{
// Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GuiLayer::createDockspace()
{
    // Create the docking environment
    // | ImGuiWindowFlags_NoTitleBar
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking |
                                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
    ImGui::PopStyleVar(3);

    ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");

    ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();
}

void GuiLayer::createPerformanceWindow()
{
    ImGui::Begin("Performance window");
    ImGui::Text("Application average (%.0f FPS)", ImGui::GetIO().Framerate);
    ImGui::End();
}


void GuiLayer::drawModelPropertiesPanel(Scene &scene, std::map<std::string, UITexture> &uiTextures, ModalManager &modalManager) {

    int tempScenePos = 0;
    int tempMeshPos = 0;
    if (!scene.models.empty() || !scene.lights.empty())
    {
        ImGui::Begin("Mesh Properties");
        {
            int i = 0;
            for (auto &model: scene.models) {
                if (model->selected) {
                    bool tNode = ImGui::TreeNodeEx((void*)"###", ImGuiTreeNodeFlags_DefaultOpen, "%s", model->modelName.c_str());
                    if (tNode) {
                        ImGui::PushID(i);
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("Name");
                        ImGui::SameLine();
                        ImGui::InputText("", model->modelName.data(), model->modelName.size()+1);
                        ImGui::PopID();


                        if (ImGui::CollapsingHeader("Mesh Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
                            ImGui::PushID(&model->uniformScale);
                            ImGui::AlignTextToFramePadding();
                            ImGui::Text("Uniform Scale");
                            ImGui::SameLine();
                            ImGui::SliderFloat("\t", &model->uniformScale, 1.0f, 20.0f);
                            ImGui::PopID();

                            ImGui::PushID(&model->scaleAxes[0]);
                            ImGui::AlignTextToFramePadding();
                            ImGui::Text("Axis Scale");
                            ImGui::SameLine();
                            ImGui::SliderFloat3("\t", &model->scaleAxes[0], 0.0, 100.0);
                            ImGui::PopID();

                            ImGui::PushID(&model->modelMatrix);
                            ImGui::AlignTextToFramePadding();
                            ImGui::Text("Model Position");
                            ImGui::SameLine();
                            ImGui::SliderFloat3("\t", &model->pos[0], -1000.0, 1000.0);
                            ImGui::PopID();

                            ImGui::PushID(&model->rotateFloats[0]);
                            ImGui::AlignTextToFramePadding();
                            ImGui::Text("Model Rotation");
                            ImGui::SameLine();
                            ImGui::SliderFloat3("\t", &model->rotateFloats[0], -1000.0, 1000.0);
                            ImGui::PopID();
                        }

                        if (ImGui::CollapsingHeader("Mesh Material", ImGuiTreeNodeFlags_DefaultOpen)) {
                            ImGui::PushID(&model->material.diffuse);
                            ImGui::AlignTextToFramePadding();
                            ImGui::Text("Diffuse");
                            ImGui::SameLine();
                            ImGui::ColorEdit3("\t", &model->material.diffuse[0]);
                            ImGui::PopID();

                            ImGui::PushID(&model->material.specular);
                            ImGui::AlignTextToFramePadding();
                            ImGui::Text("Specular");
                            ImGui::SameLine();
                            ImGui::ColorEdit3("\t", &model->material.specular[0]);
                            ImGui::PopID();

                            ImGui::PushID(&model->material.ambient);
                            ImGui::AlignTextToFramePadding();
                            ImGui::Text("Ambient");
                            ImGui::SameLine();
                            ImGui::ColorEdit3("\t", &model->material.ambient[0]);
                            ImGui::PopID();

                            ImGui::PushID(&model->meshes[0]);
                            tempScenePos = i;
                            for (int i = 0; i < model->meshes.size(); i++) {
                                if (model->meshes[i].textures.empty()) {
                                    ImGui::PushID("texture_");
                                    ImGui::Text("Add Texture");
                                    if (ImGui::ImageButton((void *) (intptr_t) uiTextures.find("default")->second.textureID, ImVec2(50, 50))) {
                                       modalManager.textureDialog.Open();
                                       tempMeshPos = i;
                                    }
                                    ImGui::PopID();
                                }
                                else {
                                    float padding = 16.0f;
                                    float cellSize = 50 + padding;

                                    float panelWidth = ImGui::GetContentRegionAvail().x;
                                    int columnCount = (int)(panelWidth / cellSize);
                                    if (columnCount < 1)
                                        columnCount = 1;


                                    ImGui::Columns(columnCount, 0, false);

                                    for (Texture &t : model->meshes[i].textures) {
                                        if (ImGui::ImageButton((void *) (intptr_t) t.id, ImVec2(50, 50)))
                                            ImGui::NextColumn();
                                    }
                                    ImGui::Columns(1);
                                }



                            }
                            ImGui::PopID();
                        }

                        ImGui::TreePop();
                    }
                }
                ++i;
            }

            for (auto &light: scene.lights) {
                if (light->selected) {
                    bool tNode = ImGui::TreeNodeEx((void*)"###", ImGuiTreeNodeFlags_DefaultOpen, "%s", light->modelName.c_str());
//                    glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
//                    glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
//                    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
//                    glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f);
//                    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
//
//                    float constant = 1.0f;
//                    float linear = 0.09f;
//                    float quadratic = 0.032f;
//
//                    float innerCutoff = glm::cos(glm::radians(12.5f));
//                    float outerCutoff = glm::cos(glm::radians(15.0f));
                    if (tNode) {
                        ImGui::PushID(i);
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("Name");
                        ImGui::SameLine();
                        ImGui::InputText("", light->modelName.data(), light->modelName.size() + 1);
                        ImGui::PopID();

                        if (ImGui::CollapsingHeader("Light Properties", ImGuiTreeNodeFlags_DefaultOpen)) {
                           ImGui::Text(light->objectType.c_str());


                           ImGui::PushID(&light->ambient);
                           ImGui::AlignTextToFramePadding();
                           ImGui::Text("Ambient");
                           ImGui::SameLine();
                           ImGui::DragFloat("\t", &light->ambient);
                           ImGui::PopID();

                           ImGui::PushID(&light->rotateFloats[0]);
                           ImGui::AlignTextToFramePadding();
                           ImGui::Text("Direction");
                           ImGui::SameLine();
                           ImGui::SliderFloat3("\t", &light->rotateFloats[0], -1000.0, 1000.0);
                           ImGui::PopID();

                            ImGui::PushID(&light->constant);
                            ImGui::AlignTextToFramePadding();
                            ImGui::Text("Constant");
                            ImGui::SameLine();
                            ImGui::SliderFloat("\t", &light->constant, 0.0f, 10.0f);
                            ImGui::PopID();

                            ImGui::PushID(&light->linear);
                            ImGui::AlignTextToFramePadding();
                            ImGui::Text("Linear");
                            ImGui::SameLine();
                            ImGui::SliderFloat("\t", &light->linear, 0.00f, 0.2f);
                            ImGui::PopID();

                            ImGui::PushID(&light->quadratic);
                            ImGui::AlignTextToFramePadding();
                            ImGui::Text("Quadratic");
                            ImGui::SameLine();
                            ImGui::SliderFloat("\t", &light->quadratic, 0.000f, 0.100f);
                            ImGui::PopID();

                            ImGui::PushID(&light->color);
                            ImGui::AlignTextToFramePadding();
                            ImGui::Text("Color");
                            ImGui::SameLine();
                            ImGui::ColorPicker3("\t", &light->color[0]);
                            ImGui::PopID();
                        }

                        if (ImGui::CollapsingHeader("Light Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
                            ImGui::PushID(&light->uniformScale);
                            ImGui::AlignTextToFramePadding();
                            ImGui::Text("Uniform Scale");
                            ImGui::SameLine();
                            ImGui::SliderFloat("\t", &light->uniformScale, 1.0f, 20.0f);
                            ImGui::PopID();

                            ImGui::PushID(&light->scaleAxes[0]);
                            ImGui::AlignTextToFramePadding();
                            ImGui::Text("Axis Scale");
                            ImGui::SameLine();
                            ImGui::SliderFloat3("\t", &light->scaleAxes[0], 0.0, 100.0);
                            ImGui::PopID();

                            ImGui::PushID(&light->modelMatrix);
                            ImGui::AlignTextToFramePadding();
                            ImGui::Text("Model Position");
                            ImGui::SameLine();
                            ImGui::SliderFloat3("\t", &light->pos[0], -1000.0, 1000.0);
                            ImGui::PopID();
                        }

                        ImGui::TreePop();
                    }
                }
                ++i;
            }
        }
        ImGui::End();
    }

    {
        modalManager.textureDialog.Display();

        if (modalManager.textureDialog.HasSelected())
        {
            std::string texturePath = modalManager.textureDialog.GetSelected().string();
            // FIXME: feels like there must be a better way
            scene.models[tempScenePos]->meshes[tempMeshPos].saveTexture(texturePath);
            modalManager.textureDialog.ClearSelected();
        }
    }
}

void GuiLayer::drawCameraPropertiesPanel(Camera &camera) {
    ImGui::Begin("Camera Properties");
    {
        ImGui::Text("Select projection mode:");
        ImGui::Text("Swap between orthographic and projection:");
        ImGui::SameLine();HelpMarker("Perspective is default.\n");
        ImGui::Checkbox("Click here", &camera.orthographic);
        ImGui::SliderFloat("FOV", &camera.fov, 45.0f, 120.0f);
        ImGui::DragFloat3("Camera Pos", &camera.pos[0]);
        ImGui::SliderFloat("Camera Speed", &camera.speed, 1.0f, 25.0f);
        ImGui::SliderFloat("Camera Sensitivity", &camera.sensitivity, 0.1f, 1.0f);
        if(ImGui::Button("Reset camera")) {
            camera.pos = camera.initialPos;
        }

    }
    ImGui::End();
}

void GuiLayer::drawDebugEventsPanel() {
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::Begin("Events");
    {
        ImGui::Text("Keys down:");          for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyDown(i))        { ImGui::SameLine(); ImGui::Text("%d (0x%X) (%.02f secs)", i, i, io.KeysDownDuration[i]); }
        ImGui::Text("Keys pressed:");       for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyPressed(i))     { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
        ImGui::Text("Keys release:");       for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyReleased(i))    { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
        ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
        ImGui::Text("Chars queue:");        for (int i = 0; i < io.InputQueueCharacters.Size; i++) { ImWchar c = io.InputQueueCharacters[i]; ImGui::SameLine();  ImGui::Text("\'%c\' (0x%04X)", (c > ' ' && c <= 255) ? (char)c : '?', c); } // FIXME: We should convert 'c' to UTF-8 here but the functions are not public.

        ImGui::Text("NavInputs down:");     for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++) if (io.NavInputs[i] > 0.0f)              { ImGui::SameLine(); ImGui::Text("[%d] %.2f (%.02f secs)", i, io.NavInputs[i], io.NavInputsDownDuration[i]); }
        ImGui::Text("NavInputs pressed:");  for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++) if (io.NavInputsDownDuration[i] == 0.0f) { ImGui::SameLine(); ImGui::Text("[%d]", i); }

        if (ImGui::IsMousePosValid())
            ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
        else
            ImGui::Text("Mouse pos: <INVALID>");
        ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
        ImGui::Text("Mouse down:");     for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDown(i))         { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
        ImGui::Text("Mouse clicked:");  for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseClicked(i))      { ImGui::SameLine(); ImGui::Text("b%d", i); }
        ImGui::Text("Mouse dblclick:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDoubleClicked(i)){ ImGui::SameLine(); ImGui::Text("b%d", i); }
        ImGui::Text("Mouse released:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseReleased(i))     { ImGui::SameLine(); ImGui::Text("b%d", i); }
        ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);
    }
    ImGui::End();
}

void GuiLayer::drawScenePanel(unsigned int &textureColorbuffer, bool &firstMouse, float &deltaTime, Scene &scene, Keymap &keymap) {
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    float lastX =  1920.0 / 2.0;
    float lastY =  1080.0 / 2.0;
    float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
    float pitch =  0.0f;
    glm::vec3 front;

    ImGui::Begin("Scene");
    {
        // Using a Child allow to fill all the space of the window.
        // It also alows customization
        ImGui::BeginChild("ViewportRender");
        // Get the size of the child (i.e. the whole draw size of the windows).
        ImVec2 wsize = ImGui::GetWindowSize();
        // Because I use the texture from OpenGL, I need to invert the V from the UV.
        ImGui::Image((ImTextureID)textureColorbuffer, wsize, ImVec2(0, 1), ImVec2(1, 0));
        if (ImGui::IsWindowHovered()) {
            if((io.KeyShift && ImGui::IsKeyPressed((keymap.keys["a"])))) {
                std::cout << "adding mesh" << std::endl;
                ImGui::OpenPopup("Add object");
            }

            {
//                    std::cout <<  io.MousePos.x << "  " << io.MousePos.y << std::endl;

                if ( ImGui::IsMouseDown(0) && io.KeyShift) {



                    if (firstMouse)
                    {
                        lastX = io.MousePos.x;
                        lastY = io.MousePos.y;
                        firstMouse = false;
                    }

                    float xoffset = io.MousePos.x - lastX;
                    float yoffset = lastY - io.MousePos.y; // reversed since y-coordinates go from bottom to top
                    lastX = io.MousePos.x;
                    lastY = io.MousePos.y;


                    xoffset *= scene.camera.sensitivity;
                    yoffset *= scene.camera.sensitivity;

                    yaw += xoffset;
                    pitch += yoffset;

                    // make sure that when pitch is out of bounds, screen doesn't get flipped
                    if (pitch > 89.0f)
                        pitch = 89.0f;
                    if (pitch < -89.0f)
                        pitch = -89.0f;

                    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
                    front.y = sin(glm::radians(pitch));
                    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
                    scene.camera.front = glm::normalize(front);
                }

                // subtract minimum bound
                auto windowSize = ImGui::GetWindowSize();
                auto minBound = ImGui::GetWindowPos();
//                    minBound.x = 0;
//                    minBound.y = 0;

                ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
                ImVec2 min = {minBound.x, minBound.y};
                ImVec2 max = {maxBound.x, maxBound.y};
                glm::vec2 bounds[2];
                bounds[0] = {minBound.x, minBound.y};
                bounds[1] = {maxBound.x, maxBound.y};

                float mx = io.MousePos.x;
                float my = io.MousePos.y;
                mx -= bounds[0].x;
                my -= bounds[0].y;

                glm::vec2 viewportSize = bounds[1] - bounds[0];

                // flip y to match texture
                my = viewportSize.y - my;

                int mouseX = (int)mx;
                int mouseY = (int)my;


//                    std::cout << mouseX << " " << mouseY << std::endl;

                glReadBuffer(GL_COLOR_ATTACHMENT1);
                int pixelData;
                glReadPixels(mouseX, mouseY, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

//                    std::cout << "pixelData: " << pixelData << std::endl;

                if (ImGui::IsMouseDown(2) && pixelData <= scene.models.size()) {
                    scene.models[pixelData]->selected = !scene.models[pixelData]->selected;
                }

            }

            if (io.MouseWheel < 0 && scene.camera.fov < 120.0f) {
                scene.camera.fov = scene.camera.fov + 5.0f;
            }
            if (io.MouseWheel > 0 && scene.camera.fov > 45.0f) {
                scene.camera.fov = scene.camera.fov - 5.0f;
            }
            float frameCamSpeed = scene.camera.speed * deltaTime;
            if (ImGui::IsKeyPressed(keymap.keys["w"])) {

                scene.camera.pos += frameCamSpeed * scene.camera.front;
            }
            if (ImGui::IsKeyPressed(keymap.keys["a"])) {
                scene.camera.pos -= frameCamSpeed * glm::normalize(glm::cross(scene.camera.front, scene.camera.up));
            }
            if (ImGui::IsKeyPressed(keymap.keys["s"])) {
                scene.camera.pos -= frameCamSpeed * scene.camera.front;

            }
            if (ImGui::IsKeyPressed(keymap.keys["d"])) {
                scene.camera.pos += frameCamSpeed * glm::normalize(glm::cross(scene.camera.front, scene.camera.up));
            }
        }
        ImGui::EndChild();
    }

    ImGui::End();
}

void showSettings(bool* p_open)
{
    if (!ImGui::Begin("About Dear ImGui", p_open))
    {
        ImGui::End();
        return;
    }
    ImGui::Text("Dear ImGui %s", ImGui::GetVersion());
    ImGui::Separator();
    ImGui::Text("By Omar Cornut and all Dear ImGui contributors.");
    ImGui::Text("Dear ImGui is licensed under the MIT License, see LICENSE for more information.");

    ImGui::End();
}

void GuiLayer::drawMenubar(Settings &settings, ModalManager &modalManager, Scene &scene) {
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    if (settings.showSettingsWindow)         { showSettings(&settings.showSettingsWindow); }

    if(io.KeyCtrl && ImGui::IsKeyPressed(settings.keymap.keys["o"]) && !settings.openFilePopup) {
        settings.openFilePopup = true;
    }
    else if(io.KeyCtrl && ImGui::IsKeyPressed(settings.keymap.keys["s"]) && !settings.openSavePopup) {
        settings.openSavePopup = true;
    }
    else if(io.KeyCtrl && ImGui::IsKeyPressed(settings.keymap.keys[","]) && !settings.openSettingsPopup) {
        settings.openSettingsPopup = true;
    }

    // menu bar
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if(ImGui::MenuItem("Open Scene", "CTRL+O")) { settings.openFilePopup = true; }
                if(ImGui::MenuItem("Save Scene", "CTRL+S")) { settings.openSavePopup = true; }
                if(ImGui::MenuItem("Settings", NULL)) { settings.openSettingsPopup = true; }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

    }

    // this is a workaround for a known bug
    if (settings.openSavePopup) {
        modalManager.saveDialog.Open();
        settings.openSavePopup = false;
    }

    if (settings.openFilePopup) {
        ImGui::OpenPopup("Open File");
        settings.openFilePopup = false;
    }

    if(settings.openSettingsPopup) {
        ImGui::OpenPopup("Settings");
        settings.openSettingsPopup = false;
    }


    if (ImGui::BeginPopupModal("Open File", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        modalManager.sceneDialog.Open();
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopupModal("Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Settings");
        ImGui::Separator();

        ImGui::SliderFloat("Near Clipping", &settings.nearClipping, 0.0f, 1.0f);
        ImGui::SliderFloat("Far Clipping", &settings.farClipping, 100.0f, 1000.0f);

        ImGui::SliderFloat("Padding", &settings.padding, 0, 32);
        ImGui::Separator();
        ImGui::Text("Key Bindings");
        for (auto const& [key, val] : settings.keymap.keys) {
            ImGui::PushID(&key);
            ImGui::Text(key.c_str());
            ImGui::SameLine(25.0f);
            ImGui::Text(std::to_string(val).c_str());
            ImGui::PopID();
        }
//        ImGui::Checkbox("Set MSAA", &useMultiSampling);


        if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Save", ImVec2(120, 0))) {
            settings.serialize("settings.yml");
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::Begin("Scene Objects");
    {
        if (!scene.models.empty()) {
            for (int i = 0; i < scene.models.size(); i++) {
                ImGui::AlignTextToFramePadding();
                ImGui::Selectable(scene.models[i]->modelName.c_str(), scene.models[i]->selected);
                if (ImGui::IsItemClicked()) {
                    std::cout << scene.models[i]->modelName.c_str() << std::endl;
                    scene.models[i]->selected = !scene.models[i]->selected;
                }
            }
        }

        if (!scene.lights.empty()) {
            for (int i = 0; i < scene.lights.size(); i++) {
                ImGui::AlignTextToFramePadding();
                ImGui::Selectable(scene.lights[i]->modelName.c_str(), scene.lights[i]->selected);
                if (ImGui::IsItemClicked()) {
                    std::cout << scene.lights[i]->modelName.c_str() << std::endl;
                    scene.lights[i]->selected = !scene.lights[i]->selected;
                }
            }
        }

        if(ImGui::Button("Add object")) {

            std::cout << "adding mesh" << std::endl;
            ImGui::OpenPopup("Add object");
        }

        // FIXME: pull into new function
        if (ImGui::BeginPopupModal("Add object", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Select a model to generate");
            ImGui::Separator();

            if (ImGui::TreeNodeEx("Primitives", ImGuiTreeNodeFlags_DefaultOpen)) {
                if (ImGui::Button("Cube")) {
                    scene.models.push_back(std::make_shared<Model>("../assets/models/primitives/cube/cube.obj"));
                }
                if (ImGui::Button("Cylinder")) {
                    scene.models.push_back(std::make_shared<Model>("../assets/models/primitives/cylinder.obj"));
                }
                if (ImGui::Button("Plane")) {
                    scene.models.push_back(std::make_shared<Model>("../assets/models/primitives/plane.obj"));
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNodeEx("Light", ImGuiTreeNodeFlags_DefaultOpen)) {

                if (ImGui::Button("Light")) {
                    scene.lights.push_back(std::make_shared<Light>("../assets/models/primitives/cube/cube.obj"));
                }
                if (ImGui::Button("Directional Light")) {
                    scene.lights.push_back(std::make_shared<DirectionalLight>("../assets/models/primitives/cone.obj"));
                }
                if (ImGui::Button("Point Light")) {
                    scene.lights.push_back(std::make_shared<PointLight>("../assets/models/primitives/cube/cube.obj"));
                }
                if (ImGui::Button("Spot Light")) {
                    scene.lights.push_back(std::make_shared<SpotLight>("../assets/models/primitives/cone.obj"));
                }
                ImGui::TreePop();
            }

            if (ImGui::Button("Import")) {
                modalManager.fileDialog.Open();
            }

            if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::SameLine();
            if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }
    }
    ImGui::End();

if (ImGui::IsKeyPressed(settings.keymap.keys["del"])) {
        std::cout << "delete mesh" << std::endl;
        scene.models.erase(
                std::remove_if(scene.models.begin(), scene.models.end(), [](std::shared_ptr<Model> const model) {
                    return model->selected;
                }),
                scene.models.end());

        scene.lights.erase(
                std::remove_if(scene.lights.begin(), scene.lights.end(), [](std::shared_ptr<Light> const light) {
                    return light->selected;
                }),
                scene.lights.end());
    }

    {

        modalManager.fileDialog.Display();

        if (modalManager.fileDialog.HasSelected())
        {
            scene.models.push_back(std::make_shared<Model>(modalManager.fileDialog.GetSelected().string()));
            modalManager.fileDialog.ClearSelected();
        }


    }

    {
        modalManager.saveDialog.Display();

        if (modalManager.saveDialog.HasSelected())
        {
            Serializer serializer = Serializer(scene.models, scene.camera);
            serializer.Serialize( modalManager.saveDialog.GetSelected().string() + ".yml");
            modalManager.saveDialog.ClearSelected();
        }
    }

    {
        modalManager.sceneDialog.Display();

        if (modalManager.sceneDialog.HasSelected())
        {
            Serializer serializer = Serializer(scene.models, scene.camera);
            scene.models = serializer.Deserialize(modalManager.sceneDialog.GetSelected().string());
            modalManager.sceneDialog.ClearSelected();
        }
    }
}

void GuiLayer::drawAssetBrowser(Settings &settings, std::map<std::string, UITexture> &uiTextures) {
    ImGui::Begin("Asset Browser");
    {

        if (settings.currentDirectory != settings.startingDirectory) {
            if (ImGui::ImageButton((void *) (intptr_t) uiTextures.find("backArrow")->second.textureID, ImVec2(20, 20))) {
                settings.currentDirectory = settings.currentDirectory.parent_path();
            }
        }

        float cellSize = settings.thumbnailSize + settings.padding;

        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1)
            columnCount = 1;


        ImGui::Columns(columnCount, 0, false);


        for(auto& p: std::filesystem::directory_iterator(settings.currentDirectory))
        {
            auto relativePath = std::filesystem::relative(p.path(), settings.startingDirectory).filename();
            ImGuiStyle& style = ImGui::GetStyle();
            if (p.is_directory())
            {
                ImGui::BeginGroup();
                {
                    ImGui::PushID(p.path().string().c_str());
                    if (ImGui::ImageButton((void *) (intptr_t) uiTextures.find("folder")->second.textureID, { settings.thumbnailSize, settings.thumbnailSize }))
                    {
                        settings.currentDirectory /= p.path();
                    }
                    ImGui::TextWrapped(relativePath.string().c_str());
                    ImGui::PopID();
                }
                ImGui::EndGroup();
            } else {
                ImGui::BeginGroup();
                {
                    ImGui::PushID(p.path().string().c_str());
                    if (p.path().string().find(".yml") != std::string::npos) {
                        ImGui::ImageButton((void *) (intptr_t) uiTextures.find("yml")->second.textureID, { settings.thumbnailSize, settings.thumbnailSize });
                    } else {
                        ImGui::ImageButton((void *) (intptr_t) uiTextures.find("file")->second.textureID, { settings.thumbnailSize, settings.thumbnailSize });
                    }
                    ImGui::Text(relativePath.string().c_str());
                    ImGui::PopID();
                }
                ImGui::EndGroup();
            }
            ImGui::NextColumn();
        }

        ImGui::Columns(1);

    }
    ImGui::End();
}
