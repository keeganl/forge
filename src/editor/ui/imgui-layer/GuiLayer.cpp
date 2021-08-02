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


void GuiLayer::drawModelPropertiesPanel(std::vector<std::shared_ptr<Model>> &scenes) {

    if (scenes.size() > 0)
    {
        ImGui::Begin("Mesh Properties");
        {
            int i = 0;
            for (auto &m: scenes) {
                if (m->selected) {
                    bool tNode = ImGui::TreeNodeEx((void*)"###", ImGuiTreeNodeFlags_DefaultOpen, "%s", m->modelName.c_str());
                    if (tNode) {
                        ImGui::PushID(i);
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("Name");
                        ImGui::SameLine();
                        ImGui::InputText("", m->modelName.data(), m->modelName.size()+1);
                        ImGui::PopID();


                        if (ImGui::CollapsingHeader("Mesh Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
                            ImGui::PushID(&m->uniformScale);
                            ImGui::AlignTextToFramePadding();
                            ImGui::Text("Uniform Scale");
                            ImGui::SameLine();
                            ImGui::SliderFloat("\t", &m->uniformScale, 1.0f, 20.0f);
                            ImGui::PopID();

                            ImGui::PushID(&m->scaleAxes[0]);
                            ImGui::AlignTextToFramePadding();
                            ImGui::Text("Axis Scale");
                            ImGui::SameLine();
                            ImGui::SliderFloat3("\t", &m->scaleAxes[0], 0.0, 100.0);
                            ImGui::PopID();

                            ImGui::PushID(&m->modelMatrix);
                            ImGui::AlignTextToFramePadding();
                            ImGui::Text("Model Position");
                            ImGui::SameLine();
                            ImGui::SliderFloat3("\t", &m->pos[0], -1000.0, 1000.0);
                            ImGui::PopID();

                            ImGui::PushID(&m->rotateFloats[0]);
                            ImGui::AlignTextToFramePadding();
                            ImGui::Text("Model Rotation");
                            ImGui::SameLine();
                            ImGui::SliderFloat3("\t", &m->rotateFloats[0], -1000.0, 1000.0);
                            ImGui::PopID();
                        }

                        if (ImGui::CollapsingHeader("Mesh Material", ImGuiTreeNodeFlags_DefaultOpen)) {
                            ImGui::PushID(&m->color);
                            ImGui::Text("Color:");
                            ImGui::SameLine();
//                            HelpMarker(
//                                    "Click on the color square to open a color picker.\n"
//                                    "CTRL+click on individual component to input value.\n");
                            ImGui::ColorPicker4("Mesh color", (float*)&m->color);
                            ImGui::PopID();

                            ImGui::PushID(&m->meshes[0]);
                            for (int i = 0; i < m->meshes.size(); i++) {
                                if (m->meshes[i].textures.empty()) {
                                    ImGui::PushID("texture_");
                                    ImGui::Text("Add Texture");
//                                    if (ImGui::ImageButton((void *) (intptr_t) default_texture, ImVec2(50, 50))) {
//                                        m->meshes[i] = saveTexture(textureDialog, m->meshes[i]);
//                                    }
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

                                    for (Texture &t : m->meshes[i].textures) {
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
        }
        ImGui::End();
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

void GuiLayer::drawScenePanel(unsigned int &textureColorbuffer, bool &firstMouse, float &deltaTime, Camera &camera, Keymap &keymap, std::vector<std::shared_ptr<Model>> &scenes) {
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

            {
//                    std::cout <<  io.MousePos.x << "  " << io.MousePos.y << std::endl;

                if ( ImGui::IsMouseDown(0) && io.KeyShift) {

                    if((io.KeyShift && ImGui::IsKeyPressed((keymap.keys["a"])))) {
                        std::cout << "adding mesh" << std::endl;
                        ImGui::OpenPopup("Add object");
                    }

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


                    xoffset *= camera.sensitivity;
                    yoffset *= camera.sensitivity;

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
                    camera.front = glm::normalize(front);
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

                if (ImGui::IsMouseDown(0) && pixelData <= scenes.size()) {
                    scenes[pixelData]->selected = !scenes[pixelData]->selected;
                }

            }

            if (io.MouseWheel < 0 && camera.fov < 120.0f) {
                camera.fov = camera.fov + 5.0f;
            }
            if (io.MouseWheel > 0 && camera.fov > 45.0f) {
                camera.fov = camera.fov - 5.0f;
            }
            float frameCamSpeed = camera.speed * deltaTime;
            if (ImGui::IsKeyPressed(keymap.keys["w"])) {

                camera.pos += frameCamSpeed * camera.front;
            }
            if (ImGui::IsKeyPressed(keymap.keys["a"])) {
                camera.pos -= frameCamSpeed * glm::normalize(glm::cross(camera.front, camera.up));
            }
            if (ImGui::IsKeyPressed(keymap.keys["s"])) {
                camera.pos -= frameCamSpeed * camera.front;

            }
            if (ImGui::IsKeyPressed(keymap.keys["d"])) {
                camera.pos += frameCamSpeed * glm::normalize(glm::cross(camera.front, camera.up));
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

void GuiLayer::drawMenubar(Keymap &keymap, ModalManager &modalManager, bool &showSettingsWindow,
                           bool &openFilePopup, bool &openSavePopup, bool &openSettingsPopup, std::vector<std::shared_ptr<Model>> &scenes, Camera &camera) {
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    if (showSettingsWindow)         { showSettings(&showSettingsWindow); }

    if(io.KeyCtrl && ImGui::IsKeyPressed(keymap.keys["o"]) && !openFilePopup) {
        openFilePopup = true;
    }
    else if(io.KeyCtrl && ImGui::IsKeyPressed(keymap.keys["s"]) && !openSavePopup) {
        openSavePopup = true;
    }
    else if(io.KeyCtrl && ImGui::IsKeyPressed(keymap.keys[","]) && !openSettingsPopup) {
        openSettingsPopup = true;
    }

    // menu bar
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if(ImGui::MenuItem("Open Scene", "CTRL+O")) { openFilePopup = true; }
                if(ImGui::MenuItem("Save Scene", "CTRL+S")) { openSavePopup = true; }
                if(ImGui::MenuItem("Settings", NULL)) { openSettingsPopup = true; }
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
    if (openSavePopup) {
        modalManager.saveDialog.Open();
        openSavePopup = false;
    }

    if (openFilePopup) {
        ImGui::OpenPopup("Open File");
        openFilePopup = false;
    }

    if(openSettingsPopup) {
        ImGui::OpenPopup("Settings");
        openSettingsPopup = false;
    }


    if (ImGui::BeginPopupModal("Open File", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        modalManager.sceneDialog.Open();
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopupModal("Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Settings");
        ImGui::Separator();
        float padding = 16.0f;
        float thumbnailSize = 48.0f;

        ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
        ImGui::SliderFloat("Padding", &padding, 0, 32);
//        ImGui::Checkbox("Set MSAA", &useMultiSampling);


        if (ImGui::Button("Revert", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Apply", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }

    ImGui::Begin("Scene Objects");
    {
        for (int i = 0; i < scenes.size(); i++) {
            ImGui::AlignTextToFramePadding();
            ImGui::Selectable(scenes[i]->modelName.c_str(), scenes[i]->selected);
            if (ImGui::IsItemClicked()) {
                std::cout << scenes[i]->modelName.c_str() << std::endl;
                scenes[i]->selected = !scenes[i]->selected;
            }
        }
        if(ImGui::Button("Add object")) {

            std::cout << "adding mesh" << std::endl;
            ImGui::OpenPopup("Add object");
        }

        if (ImGui::BeginPopupModal("Add object", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Select a model to generate");
            ImGui::Separator();


            if (ImGui::Button("Cube")) {
                scenes.push_back(std::make_shared<Model>("../assets/models/primitives/cube/cube.obj"));
            }
            if (ImGui::Button("Cylinder")) {
                scenes.push_back(std::make_shared<Model>("../assets/models/primitives/cylinder.obj"));
            }
            if (ImGui::Button("Plane")) {
                scenes.push_back(std::make_shared<Model>("../assets/models/primitives/plane.obj"));
            }
            if (ImGui::Button("Light")) {
                scenes.push_back(std::make_shared<Light>("../assets/models/primitives/cube/cube.obj"));
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

    if (ImGui::IsKeyPressed(keymap.keys["del"])) {
        std::cout << "delete mesh" << std::endl;
        scenes.erase(
                std::remove_if(scenes.begin(), scenes.end(), [](std::shared_ptr<Model> const model) {
                    return model->selected;
                }),
                scenes.end());
    }

    {

        modalManager.fileDialog.Display();

        if (modalManager.fileDialog.HasSelected())
        {
            scenes.push_back(std::make_shared<Model>(modalManager.fileDialog.GetSelected().string()));
            modalManager.fileDialog.ClearSelected();
        }


    }

    {
        modalManager.saveDialog.Display();

        if (modalManager.saveDialog.HasSelected())
        {
            Serializer serializer = Serializer(scenes, camera);
            serializer.Serialize( modalManager.saveDialog.GetSelected().string() + ".yml");
            modalManager.saveDialog.ClearSelected();
        }
    }

    {
        modalManager.sceneDialog.Display();

        if (modalManager.sceneDialog.HasSelected())
        {
            Serializer serializer = Serializer(scenes, camera);
            scenes = serializer.Deserialize(modalManager.sceneDialog.GetSelected().string());
            modalManager.sceneDialog.ClearSelected();
        }
    }
}
