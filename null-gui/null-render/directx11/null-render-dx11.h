#pragma once
#pragma comment (lib, "d3d11.lib")
#include <d3d11.h>

#include "../null-render.h"

namespace null_render {
	namespace directx11 {
		ID3D11RenderTargetView* main_render_target_view;
		ID3D11Device* device;
		ID3D11DeviceContext* context;
		IDXGIFactory* factory;
		ID3D11Buffer* vertex_buffer;
		ID3D11Buffer* index_buffer;
		ID3D11InputLayout* input_layout;
		ID3D11SamplerState* font_sampler;
		ID3D11ShaderResourceView* font_texture_view;
		ID3D11RasterizerState* rasterizer_state;
		ID3D11BlendState* blend_state;
		ID3D11DepthStencilState* depth_stencil_state;
		IDXGISwapChain* swap_chain;

		bool init(ID3D11Device* _device = nullptr, ID3D11DeviceContext* _context = nullptr);
		void begin_frame();
		void shutdown();

		void render_draw_data(helpers::draw_data* _draw_data = &null_render::draw_data);
		void setup_render_state(helpers::draw_data* _draw_data);
		void create_fonts_texture();
		bool create_device_objects();
		void invalidate_device_objects();
	}
}