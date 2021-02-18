#include "UIRenderPass.h"

#include "ImGUI\imgui.h"
#include "ImGUI\imgui_impl_win32.h"
#include "ImGUI\imgui_impl_dx11.h"

namespace Engine 
{

UIRenderPass::UIRenderPass(std::vector<RenderTexture*> outputs) : RenderPass(nullptr, outputs)
{
}

void UIRenderPass::Render(const GraphicsDevice& device, Scene* scene)
{
   assert(m_outputs.size() == 1);
   
   device.Context()->OMSetRenderTargets(1, &m_outputs[0]->m_pRenderTargetView, device.pDepthStencilView);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

   for (const auto& entity : scene->GetEntities())
   {
      entity->DrawUI();
   }

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
   device.UnBindResources();
}

} // namespace Engine
