#include "shaders.h"

inline unsigned int align16(unsigned int size) {
    return ((size + 15) / 16) * 16;
}

namespace null_render {
    namespace shaders {
        void resources::init() {
            HRESULT result;
            D3D11_TEXTURE2D_DESC texture_desc;

            if (!texture) {
                back_buffer_call([&texture_desc](ID3D11Texture2D* buffer) { buffer->GetDesc(&texture_desc); });
                result = directx11::device->CreateTexture2D(&texture_desc, NULL, &texture);
            }

            if (!render_target)
                result = directx11::device->CreateRenderTargetView(texture, NULL, &render_target);

            if (!shader_resource) {
                back_buffer_call([&texture_desc](ID3D11Texture2D* buffer) { buffer->GetDesc(&texture_desc); });

                D3D11_SHADER_RESOURCE_VIEW_DESC resource_view_desc;
                resource_view_desc.Format = texture_desc.Format;
                resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                resource_view_desc.Texture2D.MostDetailedMip = 0;
                resource_view_desc.Texture2D.MipLevels = 1;

                result = directx11::device->CreateShaderResourceView(texture, &resource_view_desc, &shader_resource);
            }
        }

        void resources::clear() {
            if (texture) { texture->Release(); texture = nullptr; }
            if (render_target) { render_target->Release(); render_target = nullptr; }
            if (shader_resource) { shader_resource->Release(); shader_resource = nullptr; }
        }

        void resources::set_render_target() {
            directx11::context->OMSetRenderTargets(1, &render_target, nullptr);
        }

        void resources::get_render_target() {
            directx11::context->OMGetRenderTargets(1, &render_target, nullptr);
        }

        void resources::clear_render_target() {
            float bg[4] = { 0.f, 0.f, 0.f, 1.f };
            directx11::context->ClearRenderTargetView(render_target, bg);
        }

        shader_control::shader_control(const void* shader_source, SIZE_T source_size, size_t constant_size) {
            init(shader_source, source_size, constant_size);
        }

        void shader_control::init(const void* shader_source, size_t source_size, size_t constant_size) {
            without_constant = constant_size == 0;

            switch (type) {
            case shader_type::pixel: if (!pixel_shader) directx11::device->CreatePixelShader(shader_source, source_size, 0, &pixel_shader); break;
            case shader_type::vertex: if (!vertex_shader) directx11::device->CreateVertexShader(shader_source, source_size, 0, &vertex_shader); break;
            }

            if (!constant_buffer && !without_constant) {
                D3D11_BUFFER_DESC desc;
                desc.ByteWidth = align16(constant_size);
                desc.Usage = D3D11_USAGE_DYNAMIC;
                desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                desc.MiscFlags = 0;
                directx11::device->CreateBuffer(&desc, NULL, &constant_buffer);
            }
        }

        void shader_control::set_shader() {
            switch (type) {
            case shader_type::pixel: directx11::context->PSSetShader(pixel_shader, nullptr, 0); break;
            case shader_type::vertex: directx11::context->VSSetShader(vertex_shader, nullptr, 0); break;
            }
        }

        void shader_control::set_constant() {
            switch (type) {
            case shader_type::pixel: directx11::context->PSSetConstantBuffers(0, 1, &constant_buffer); break;
            case shader_type::vertex: directx11::context->VSSetConstantBuffers(0, 1, &constant_buffer); break;
            }
        }

        void shader_control::clear() {
            if (pixel_shader) { pixel_shader->Release(); pixel_shader = nullptr; }
            if (vertex_shader) { vertex_shader->Release(); vertex_shader = nullptr; }
            if (constant_buffer) { constant_buffer->Release(); constant_buffer = nullptr; }
        }

        void back_buffer_call(std::function<void(ID3D11Texture2D*)> call) {
            ID3D11Texture2D* backbuffer;
            directx11::swap_chain->GetBuffer(0, IID_PPV_ARGS(&backbuffer));
            call(backbuffer);
            backbuffer->Release(); backbuffer = nullptr;
        }

        void begin_frame() {
            blur::clear_all_shaders();
        }

        void setup_render_state(helpers::draw_data* _draw_data) {
            vertex::shader.set_shader();
            vertex::shader.set_constant();
            pixel::shader.set_shader();
        }

        void create_shaders() {
            vertex::create_shader_control();
            pixel::create_shader_control();

            blur::create_shader_control();
            blur::clear_layers();
            blur::clear_all_shaders();
        }

        void clear_shaders() {
            vertex::clear_shader_control();
            pixel::clear_shader_control();

            blur::clear_shader_control();
            blur::clear_layers();
            blur::clear_all_shaders();
        }

        void win_proc() {
            blur::clear_layers();
        }
    }
}