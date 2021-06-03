#include <stdio.h>
#include <d3dcompiler.h>
#ifdef _MSC_VER
#pragma comment(lib, "d3dcompiler")
#endif

#include "null-render-dx11.h"
#include "shaders\shaders.h"

int vertex_buffer_size = 5000, index_buffer_size = 10000;

struct vertex {
    float    pos[3];
    unsigned int clr;
    float    uv[2];
};

struct directx11_state {
    UINT ScissorRectsCount, ViewportsCount;
    D3D11_RECT ScissorRects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
    D3D11_VIEWPORT Viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
    ID3D11RasterizerState* RS;
    ID3D11BlendState* BlendState;
    FLOAT BlendFactor[4];
    UINT SampleMask;
    UINT StencilRef;
    ID3D11DepthStencilState* DepthStencilState;
    ID3D11ShaderResourceView* PSShaderResource;
    ID3D11SamplerState* PSSampler;
    ID3D11PixelShader* PS;
    ID3D11VertexShader* VS;
    ID3D11GeometryShader* GS;
    UINT PSInstancesCount, VSInstancesCount, GSInstancesCount;
    ID3D11ClassInstance* PSInstances[256], * VSInstances[256], * GSInstances[256];
    D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology;
    ID3D11Buffer* IndexBuffer, *VertexBuffer, *VSConstantBuffer;
    UINT IndexBufferOffset, VertexBufferStride, VertexBufferOffset;
    DXGI_FORMAT IndexBufferFormat;
    ID3D11InputLayout* InputLayout;
};

namespace null_render {
    namespace directx11 {
        bool init(ID3D11Device* _device, ID3D11DeviceContext* _context) {
            if (_device) device = _device;
            if (_context) context = _context;
            IDXGIDevice* dxg_device = nullptr;
            IDXGIAdapter* dxgi_adapter = nullptr;
            IDXGIFactory* local_factory = nullptr;

            if (device->QueryInterface(IID_PPV_ARGS(&dxg_device)) == S_OK)
                if (dxg_device->GetParent(IID_PPV_ARGS(&dxgi_adapter)) == S_OK)
                    if (dxgi_adapter->GetParent(IID_PPV_ARGS(&local_factory)) == S_OK) {
                        factory = local_factory;
                    }
            if (dxg_device) dxg_device->Release();
            if (dxgi_adapter) dxgi_adapter->Release();
            device->AddRef();
            context->AddRef();

            return true;
        }

        void begin_frame() {
            shaders::begin_frame();
            if (!font_sampler) create_device_objects();
        }

        void shutdown() {
            invalidate_device_objects();
            if (factory) { factory->Release(); factory = NULL; }
            if (device) { device->Release(); device = NULL; }
            if (context) { context->Release(); context = NULL; }
        }

        void render_draw_data(helpers::draw_data* _draw_data) {
            if (_draw_data->display_size.x <= 0.0f || _draw_data->display_size.y <= 0.0f)
                return;

            if (!vertex_buffer || vertex_buffer_size < _draw_data->total_vtx_count) {
                if (vertex_buffer) { vertex_buffer->Release(); vertex_buffer = NULL; }
                vertex_buffer_size = _draw_data->total_vtx_count + 5000;
                D3D11_BUFFER_DESC desc;
                memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
                desc.Usage = D3D11_USAGE_DYNAMIC;
                desc.ByteWidth = vertex_buffer_size * sizeof(vertex);
                desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                desc.MiscFlags = 0;
                if (device->CreateBuffer(&desc, NULL, &vertex_buffer) < 0)
                    return;
            }
            if (!index_buffer || index_buffer_size < _draw_data->total_idx_count) {
                if (index_buffer) { index_buffer->Release(); index_buffer = NULL; }
                index_buffer_size = _draw_data->total_idx_count + 10000;
                D3D11_BUFFER_DESC desc;
                memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
                desc.Usage = D3D11_USAGE_DYNAMIC;
                desc.ByteWidth = index_buffer_size * sizeof(unsigned short);
                desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
                desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                if (device->CreateBuffer(&desc, NULL, &index_buffer) < 0)
                    return;
            }

            D3D11_MAPPED_SUBRESOURCE vtx_resource, idx_resource;
            if (context->Map(vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vtx_resource) != S_OK) return;
            if (context->Map(index_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &idx_resource) != S_OK) return;
            vertex* vtx_dst = (vertex*)vtx_resource.pData;
            unsigned short* idx_dst = (unsigned short*)idx_resource.pData;
            for (int n = 0; n < _draw_data->cmd_lists_count; n++) {
                const draw_list* cmd_list = _draw_data->cmd_lists[n];
                helpers::vertex* vtx_src = (helpers::vertex*)cmd_list->vtx_buffer.data();
                for (int i = 0; i < cmd_list->vtx_buffer.size(); i++) {
                    vtx_dst->pos[0] = vtx_src->pos.x;
                    vtx_dst->pos[1] = vtx_src->pos.y;
                    vtx_dst->pos[2] = 0.0f;
                    vtx_dst->clr = vtx_src->clr.to_unsigned();
                    vtx_dst->uv[0] = vtx_src->uv.x;
                    vtx_dst->uv[1] = vtx_src->uv.y;
                    vtx_dst++;
                    vtx_src++;
                }
                memcpy(idx_dst, cmd_list->idx_buffer.data(), cmd_list->idx_buffer.size() * sizeof(unsigned short));
                idx_dst += cmd_list->idx_buffer.size();
            }
            context->Unmap(vertex_buffer, 0);
            context->Unmap(index_buffer, 0);


            {
                float L = _draw_data->display_pos.x;
                float R = _draw_data->display_pos.x + _draw_data->display_size.x;
                float T = _draw_data->display_pos.y;
                float B = _draw_data->display_pos.y + _draw_data->display_size.y;
                float matrix[4][4] =
                {
                    { 2.0f / (R - L),   0.0f,           0.0f,       0.0f },
                    { 0.0f,         2.0f / (T - B),     0.0f,       0.0f },
                    { 0.0f,         0.0f,           0.5f,       0.0f },
                    { (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f },
                };

                shaders::vertex::constant vertex_const;
                memcpy(&vertex_const.matrix, matrix, sizeof(matrix));

                shaders::vertex::shader.edit_constant(vertex_const);
            }

            directx11_state old = {};
            old.ScissorRectsCount = old.ViewportsCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
            context->RSGetScissorRects(&old.ScissorRectsCount, old.ScissorRects);
            context->RSGetViewports(&old.ViewportsCount, old.Viewports);
            context->RSGetState(&old.RS);
            context->OMGetBlendState(&old.BlendState, old.BlendFactor, &old.SampleMask);
            context->OMGetDepthStencilState(&old.DepthStencilState, &old.StencilRef);
            context->PSGetShaderResources(0, 1, &old.PSShaderResource);
            context->PSGetSamplers(0, 1, &old.PSSampler);
            old.PSInstancesCount = old.VSInstancesCount = old.GSInstancesCount = 256;
            context->PSGetShader(&old.PS, old.PSInstances, &old.PSInstancesCount);
            context->VSGetShader(&old.VS, old.VSInstances, &old.VSInstancesCount);
            context->VSGetConstantBuffers(0, 1, &old.VSConstantBuffer);
            context->GSGetShader(&old.GS, old.GSInstances, &old.GSInstancesCount);

            context->IAGetPrimitiveTopology(&old.PrimitiveTopology);
            context->IAGetIndexBuffer(&old.IndexBuffer, &old.IndexBufferFormat, &old.IndexBufferOffset);
            context->IAGetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset);
            context->IAGetInputLayout(&old.InputLayout);

            setup_render_state(_draw_data);

            int global_idx_offset = 0;
            int global_vtx_offset = 0;
            vec2 clip_off = _draw_data->display_pos;
            unsigned int _vtx_offset = 0;
            unsigned int _idx_offset = 0;
            unsigned int _elem_count = 0;
            for (int n = 0; n < _draw_data->cmd_lists_count; n++) {
                draw_list* cmd_list = _draw_data->cmd_lists[n];
                for (int cmd_i = 0; cmd_i < cmd_list->cmd_buffer.size(); cmd_i++) {
                    helpers::cmd* pcmd = &cmd_list->cmd_buffer[cmd_i];
                    if (pcmd->callback != nullptr) {
                        pcmd->callback(pcmd);
                        continue;
                    }

                    if (pcmd->setup_render_state_update_call == true) {
                        setup_render_state(_draw_data);
                        continue;
                    }

                    const D3D11_RECT r = { (LONG)(pcmd->clip_rect.min.x - clip_off.x), (LONG)(pcmd->clip_rect.min.y - clip_off.y), (LONG)(pcmd->clip_rect.max.x - clip_off.x), (LONG)(pcmd->clip_rect.max.y - clip_off.y) };
                    context->RSSetScissorRects(1, &r);

                    ID3D11ShaderResourceView* texture_srv = (ID3D11ShaderResourceView*)pcmd->texture_id;
                    context->PSSetSamplers(0, 1, &font_sampler);
                    context->PSSetShaderResources(0, 1, &texture_srv);
                    context->DrawIndexed(pcmd->elem_count, pcmd->idx_offset + global_idx_offset, pcmd->vtx_offset + global_vtx_offset);
                    if (!pcmd->setup_render_state_update_call) context->PSSetShader(shaders::pixel::shader.pixel_shader, nullptr, 0);
                }
                global_idx_offset += cmd_list->idx_buffer.size();
                global_vtx_offset += cmd_list->vtx_buffer.size();
            }

            context->RSSetScissorRects(old.ScissorRectsCount, old.ScissorRects);
            context->RSSetViewports(old.ViewportsCount, old.Viewports);
            context->RSSetState(old.RS); if (old.RS) old.RS->Release();
            context->OMSetBlendState(old.BlendState, old.BlendFactor, old.SampleMask); if (old.BlendState) old.BlendState->Release();
            context->OMSetDepthStencilState(old.DepthStencilState, old.StencilRef); if (old.DepthStencilState) old.DepthStencilState->Release();
            context->PSSetShaderResources(0, 1, &old.PSShaderResource); if (old.PSShaderResource) old.PSShaderResource->Release();
            context->PSSetSamplers(0, 1, &old.PSSampler); if (old.PSSampler) old.PSSampler->Release();
            context->PSSetShader(old.PS, old.PSInstances, old.PSInstancesCount); if (old.PS) old.PS->Release();
            for (UINT i = 0; i < old.PSInstancesCount; i++) if (old.PSInstances[i]) old.PSInstances[i]->Release();
            context->VSSetShader(old.VS, old.VSInstances, old.VSInstancesCount); if (old.VS) old.VS->Release();
            context->VSSetConstantBuffers(0, 1, &old.VSConstantBuffer); if (old.VSConstantBuffer) old.VSConstantBuffer->Release();
            context->GSSetShader(old.GS, old.GSInstances, old.GSInstancesCount); if (old.GS) old.GS->Release();
            for (UINT i = 0; i < old.VSInstancesCount; i++) if (old.VSInstances[i]) old.VSInstances[i]->Release();
            context->IASetPrimitiveTopology(old.PrimitiveTopology);
            context->IASetIndexBuffer(old.IndexBuffer, old.IndexBufferFormat, old.IndexBufferOffset); if (old.IndexBuffer) old.IndexBuffer->Release();
            context->IASetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset); if (old.VertexBuffer) old.VertexBuffer->Release();
            context->IASetInputLayout(old.InputLayout); if (old.InputLayout) old.InputLayout->Release();
        }

        void setup_render_state(helpers::draw_data* _draw_data) {
            D3D11_VIEWPORT vp;
            memset(&vp, 0, sizeof(D3D11_VIEWPORT));
            vp.Width = _draw_data->display_size.x;
            vp.Height = _draw_data->display_size.y;
            vp.MinDepth = 0.0f;
            vp.MaxDepth = 1.0f;
            vp.TopLeftX = vp.TopLeftY = 0;
            context->RSSetViewports(1, &vp);

            shaders::setup_render_state(_draw_data);

            unsigned int stride = sizeof(vertex);
            unsigned int offset = 0;
            context->IASetInputLayout(input_layout);
            context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
            context->IASetIndexBuffer(index_buffer, sizeof(unsigned short) == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
            context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            context->PSSetSamplers(0, 1, &font_sampler);
            context->GSSetShader(NULL, NULL, 0);
            context->HSSetShader(NULL, NULL, 0);
            context->DSSetShader(NULL, NULL, 0);
            context->CSSetShader(NULL, NULL, 0);

            const float blend_factor[4] = { 0.f, 0.f, 0.f, 0.f };
            context->OMSetBlendState(blend_state, blend_factor, 0xffffffff);
            context->OMSetDepthStencilState(depth_stencil_state, 0);
            context->RSSetState(rasterizer_state);
        }

        void create_fonts_texture() {
            unsigned char* pixels;
            int width, height;
            null_font::vars::font_atlas->get_tex_data_as_rgba32(&pixels, &width, &height);

            {
                D3D11_TEXTURE2D_DESC desc;
                ZeroMemory(&desc, sizeof(desc));
                desc.Width = width;
                desc.Height = height;
                desc.MipLevels = 1;
                desc.ArraySize = 1;
                desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                desc.SampleDesc.Count = 1;
                desc.Usage = D3D11_USAGE_DEFAULT;
                desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
                desc.CPUAccessFlags = 0;

                ID3D11Texture2D* pTexture = NULL;
                D3D11_SUBRESOURCE_DATA subResource;
                subResource.pSysMem = pixels;
                subResource.SysMemPitch = desc.Width * 4;
                subResource.SysMemSlicePitch = 0;
                device->CreateTexture2D(&desc, &subResource, &pTexture);

                D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
                ZeroMemory(&srvDesc, sizeof(srvDesc));
                srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                srvDesc.Texture2D.MipLevels = desc.MipLevels;
                srvDesc.Texture2D.MostDetailedMip = 0;
                device->CreateShaderResourceView(pTexture, &srvDesc, &font_texture_view);
                pTexture->Release();
            }

            null_font::vars::font_atlas->set_tex_id((void*)font_texture_view);

            {
                D3D11_SAMPLER_DESC desc;
                ZeroMemory(&desc, sizeof(desc));
                desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
                desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
                desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
                desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
                desc.MipLODBias = 0.f;
                desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
                desc.MinLOD = 0.f;
                desc.MaxLOD = 0.f;

                device->CreateSamplerState(&desc, &font_sampler);
            }
        }

        void invalidate_device_objects() {
            if (!device) return;

            shaders::clear_shaders();

            if (font_sampler) { font_sampler->Release(); font_sampler = NULL; }
            if (font_texture_view) { font_texture_view->Release(); font_texture_view = NULL; null_font::vars::font_atlas->set_tex_id(NULL); }
            if (index_buffer) { index_buffer->Release(); index_buffer = NULL; }
            if (vertex_buffer) { vertex_buffer->Release(); vertex_buffer = NULL; }

            if (blend_state) { blend_state->Release(); blend_state = NULL; }
            if (depth_stencil_state) { depth_stencil_state->Release(); depth_stencil_state = NULL; }
            if (rasterizer_state) { rasterizer_state->Release(); rasterizer_state = NULL; }
            if (input_layout) { input_layout->Release(); input_layout = NULL; }
        }

        bool create_device_objects() {
            if (!device) return false;
            if (font_sampler) invalidate_device_objects();

            shaders::create_shaders();

            {

                D3D11_INPUT_ELEMENT_DESC local_layout[] = {
                    { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (UINT)offsetof(vertex, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
                    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (UINT)offsetof(vertex, uv),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
                    { "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, (UINT)offsetof(vertex, clr), D3D11_INPUT_PER_VERTEX_DATA, 0 },
                };
                device->CreateInputLayout(local_layout, 3, shaders::shader_sources::vertex, sizeof(shaders::shader_sources::vertex), &input_layout);
            }


            {
                D3D11_BLEND_DESC desc;
                ZeroMemory(&desc, sizeof(desc));
                desc.AlphaToCoverageEnable = false;
                desc.RenderTarget[0].BlendEnable = true;
                desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
                desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
                desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
                desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
                desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
                desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
                desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
                device->CreateBlendState(&desc, &blend_state);
            }

            {
                D3D11_RASTERIZER_DESC desc;
                ZeroMemory(&desc, sizeof(desc));
                desc.FillMode = D3D11_FILL_SOLID;
                desc.CullMode = D3D11_CULL_NONE;
                desc.ScissorEnable = true;
                desc.DepthClipEnable = true;
                device->CreateRasterizerState(&desc, &rasterizer_state);
            }

            {
                D3D11_DEPTH_STENCIL_DESC desc;
                ZeroMemory(&desc, sizeof(desc));
                desc.DepthEnable = false;
                desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
                desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
                desc.StencilEnable = false;
                desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp = desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
                desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
                desc.BackFace = desc.FrontFace;
                device->CreateDepthStencilState(&desc, &depth_stencil_state);
            }

            create_fonts_texture();

            return true;
        }
    }
}