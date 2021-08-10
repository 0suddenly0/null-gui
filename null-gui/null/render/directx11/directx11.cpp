#include <stdio.h>
#include <d3dcompiler.h>
#ifdef _MSC_VER
#pragma comment(lib, "d3dcompiler")
#endif

#include "directx11.h"
#include "shaders\shaders.h"

int vertex_buffer_size = 5000, index_buffer_size = 10000;

struct directx11_state {
    UINT scissor_rects_count, viewports_count;
    D3D11_RECT scissor_rects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
    D3D11_VIEWPORT viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
    ID3D11RasterizerState* rasterizer_state;
    ID3D11BlendState* blend_state;
    FLOAT blend_factor[4];
    UINT sample_mask;
    UINT stencil_ref;
    ID3D11DepthStencilState* depth_stencil_state;
    ID3D11ShaderResourceView* shader_resource;
    ID3D11SamplerState* sampler;
    ID3D11PixelShader* pixel_shader;
    ID3D11VertexShader* vertex_shader;
    ID3D11GeometryShader* geometry_shader;
    UINT pixel_shader_instances_count, vertex_shader_instances_count, geometry_shader_instances_count;
    ID3D11ClassInstance* pixel_shader_instances[256], * vertex_shader_instances[256], * geometry_shader_instances[256];
    D3D11_PRIMITIVE_TOPOLOGY primitive_topology;
    ID3D11Buffer* index_buffer, *vertex_buffer, *vertex_shader_constant_buffer;
    UINT index_buffer_offset, vertex_buffer_stride, vertex_buffer_offset;
    DXGI_FORMAT index_buffer_format;
    ID3D11InputLayout* input_layout;
};

namespace null {
    namespace render {
        namespace directx11 {
            bool initialize(ID3D11Device* _device, ID3D11DeviceContext* _context) {
                if(_device) device = _device;
                if(_context) context = _context;
                IDXGIDevice* dxg_device = nullptr;
                IDXGIAdapter* dxgi_adapter = nullptr;
                IDXGIFactory* local_factory = nullptr;

                if(device->QueryInterface(IID_PPV_ARGS(&dxg_device)) == S_OK)
                    if(dxg_device->GetParent(IID_PPV_ARGS(&dxgi_adapter)) == S_OK)
                        if(dxgi_adapter->GetParent(IID_PPV_ARGS(&local_factory)) == S_OK) {
                            factory = local_factory;
                        }
                if(dxg_device) dxg_device->Release();
                if(dxgi_adapter) dxgi_adapter->Release();
                device->AddRef();
                context->AddRef();

                return true;
            }

            void begin_frame() {
                shaders::begin_frame();
                if(!font_sampler) create_device_objects();
            }

            void shutdown() {
                invalidate_device_objects();
                if(factory) { factory->Release(); factory = NULL; }
                if(device) { device->Release(); device = NULL; }
                if(context) { context->Release(); context = NULL; }
            }

            void render_draw_data(detail::draw_data* _draw_data) {
                if(_draw_data->display_size.x <= 0.0f || _draw_data->display_size.y <= 0.0f)
                    return;

                if(!vertex_buffer || vertex_buffer_size < _draw_data->total_vtx_count) {
                    if(vertex_buffer) { vertex_buffer->Release(); vertex_buffer = NULL; }
                    vertex_buffer_size = _draw_data->total_vtx_count + 5000;
                    D3D11_BUFFER_DESC desc;
                    memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
                    desc.Usage = D3D11_USAGE_DYNAMIC;
                    desc.ByteWidth = vertex_buffer_size * sizeof(vertex);
                    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                    desc.MiscFlags = 0;
                    if(device->CreateBuffer(&desc, NULL, &vertex_buffer) < 0)
                        return;
                }
                if(!index_buffer || index_buffer_size < _draw_data->total_idx_count) {
                    if(index_buffer) { index_buffer->Release(); index_buffer = NULL; }
                    index_buffer_size = _draw_data->total_idx_count + 10000;
                    D3D11_BUFFER_DESC desc;
                    memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
                    desc.Usage = D3D11_USAGE_DYNAMIC;
                    desc.ByteWidth = index_buffer_size * sizeof(unsigned short);
                    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
                    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                    if(device->CreateBuffer(&desc, NULL, &index_buffer) < 0)
                        return;
                }

                D3D11_MAPPED_SUBRESOURCE vtx_resource, idx_resource;
                if(context->Map(vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vtx_resource) != S_OK) return;
                if(context->Map(index_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &idx_resource) != S_OK) return;
                vertex* vtx_dst = (vertex*)vtx_resource.pData;
                unsigned short* idx_dst = (unsigned short*)idx_resource.pData;
                for(int n = 0; n < _draw_data->cmd_lists_count; n++) {
                    const draw_list* cmd_list = _draw_data->cmd_lists[n];
                    detail::vertex* vtx_src = (detail::vertex*)cmd_list->vtx_buffer.data();
                    for(int i = 0; i < cmd_list->vtx_buffer.size(); i++) {
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
                old.scissor_rects_count = old.viewports_count = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
                context->RSGetScissorRects(&old.scissor_rects_count, old.scissor_rects);
                context->RSGetViewports(&old.viewports_count, old.viewports);
                context->RSGetState(&old.rasterizer_state);
                context->OMGetBlendState(&old.blend_state, old.blend_factor, &old.sample_mask);
                context->OMGetDepthStencilState(&old.depth_stencil_state, &old.stencil_ref);
                context->PSGetShaderResources(0, 1, &old.shader_resource);
                context->PSGetSamplers(0, 1, &old.sampler);
                old.pixel_shader_instances_count = old.vertex_shader_instances_count = old.geometry_shader_instances_count = 256;
                context->PSGetShader(&old.pixel_shader, old.pixel_shader_instances, &old.pixel_shader_instances_count);
                context->VSGetShader(&old.vertex_shader, old.vertex_shader_instances, &old.vertex_shader_instances_count);
                context->VSGetConstantBuffers(0, 1, &old.vertex_shader_constant_buffer);
                context->GSGetShader(&old.geometry_shader, old.geometry_shader_instances, &old.geometry_shader_instances_count);

                context->IAGetPrimitiveTopology(&old.primitive_topology);
                context->IAGetIndexBuffer(&old.index_buffer, &old.index_buffer_format, &old.index_buffer_offset);
                context->IAGetVertexBuffers(0, 1, &old.vertex_buffer, &old.vertex_buffer_stride, &old.vertex_buffer_offset);
                context->IAGetInputLayout(&old.input_layout);

                setup_render_state(_draw_data);

                int global_idx_offset = 0;
                int global_vtx_offset = 0;
                vec2 clip_off = _draw_data->display_pos;
                unsigned int _vtx_offset = 0;
                unsigned int _idx_offset = 0;
                unsigned int _elem_count = 0;
                for(int n = 0; n < _draw_data->cmd_lists_count; n++) {
                    draw_list* cmd_list = _draw_data->cmd_lists[n];
                    for(int cmd_i = 0; cmd_i < cmd_list->cmd_buffer.size(); cmd_i++) {
                        detail::cmd* pcmd = &cmd_list->cmd_buffer[cmd_i];
                        if(pcmd->callback != nullptr) {
                            pcmd->callback(pcmd);
                            continue;
                        }

                        if(pcmd->setup_render_state_update_call == true) {
                            setup_render_state(_draw_data);
                            continue;
                        }

                        const D3D11_RECT r = { (LONG)(pcmd->clip_rect.min.x - clip_off.x), (LONG)(pcmd->clip_rect.min.y - clip_off.y), (LONG)(pcmd->clip_rect.max.x - clip_off.x), (LONG)(pcmd->clip_rect.max.y - clip_off.y) };
                        context->RSSetScissorRects(1, &r);

                        ID3D11ShaderResourceView* texture_srv = (ID3D11ShaderResourceView*)pcmd->texture_id;
                        context->PSSetShaderResources(0, 1, &texture_srv);
                        context->DrawIndexed(pcmd->elem_count, pcmd->idx_offset + global_idx_offset, pcmd->vtx_offset + global_vtx_offset);
                    }
                    global_idx_offset += cmd_list->idx_buffer.size();
                    global_vtx_offset += cmd_list->vtx_buffer.size();
                }

                context->RSSetScissorRects(old.scissor_rects_count, old.scissor_rects);
                context->RSSetViewports(old.viewports_count, old.viewports);
                context->RSSetState(old.rasterizer_state); if(old.rasterizer_state) old.rasterizer_state->Release();
                context->OMSetBlendState(old.blend_state, old.blend_factor, old.sample_mask); if(old.blend_state) old.blend_state->Release();
                context->OMSetDepthStencilState(old.depth_stencil_state, old.stencil_ref); if(old.depth_stencil_state) old.depth_stencil_state->Release();
                context->PSSetShaderResources(0, 1, &old.shader_resource); if(old.shader_resource) old.shader_resource->Release();
                context->PSSetSamplers(0, 1, &old.sampler); if(old.sampler) old.sampler->Release();
                context->PSSetShader(old.pixel_shader, old.pixel_shader_instances, old.pixel_shader_instances_count); if(old.pixel_shader) old.pixel_shader->Release();
                for(UINT i = 0; i < old.pixel_shader_instances_count; i++) if(old.pixel_shader_instances[i]) old.pixel_shader_instances[i]->Release();
                context->VSSetShader(old.vertex_shader, old.vertex_shader_instances, old.vertex_shader_instances_count); if(old.vertex_shader) old.vertex_shader->Release();
                context->VSSetConstantBuffers(0, 1, &old.vertex_shader_constant_buffer); if(old.vertex_shader_constant_buffer) old.vertex_shader_constant_buffer->Release();
                context->GSSetShader(old.geometry_shader, old.geometry_shader_instances, old.geometry_shader_instances_count); if(old.geometry_shader) old.geometry_shader->Release();
                for(UINT i = 0; i < old.vertex_shader_instances_count; i++) if(old.vertex_shader_instances[i]) old.vertex_shader_instances[i]->Release();
                context->IASetPrimitiveTopology(old.primitive_topology);
                context->IASetIndexBuffer(old.index_buffer, old.index_buffer_format, old.index_buffer_offset); if(old.index_buffer) old.index_buffer->Release();
                context->IASetVertexBuffers(0, 1, &old.vertex_buffer, &old.vertex_buffer_stride, &old.vertex_buffer_offset); if(old.vertex_buffer) old.vertex_buffer->Release();
                context->IASetInputLayout(old.input_layout); if(old.input_layout) old.input_layout->Release();
            }

            void setup_render_state(detail::draw_data* _draw_data) {
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
                font::font_atlas->get_tex_data_as_rgba32(&pixels, &width, &height);

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

                font::font_atlas->set_tex_id((void*)font_texture_view);

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

            bool create_device_objects() {
                if(!device) return false;
                if(font_sampler) invalidate_device_objects();

                shaders::create_shaders();

                {
                    D3D11_INPUT_ELEMENT_DESC local_layout[] = {
                        { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (UINT)offsetof(vertex, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
                        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (UINT)offsetof(vertex, uv),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
                        { "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, (UINT)offsetof(vertex, clr), D3D11_INPUT_PER_VERTEX_DATA, 0 },
                    };
                    device->CreateInputLayout(local_layout, 3, shaders::sources::vertex, sizeof(shaders::sources::vertex), &input_layout);
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

            void invalidate_device_objects() {
                if(!device) return;

                shaders::clear_shaders();

                if(font_sampler) { font_sampler->Release(); font_sampler = NULL; }
                if(font_texture_view) { font_texture_view->Release(); font_texture_view = NULL; font::font_atlas->set_tex_id(NULL); }
                if(index_buffer) { index_buffer->Release(); index_buffer = NULL; }
                if(vertex_buffer) { vertex_buffer->Release(); vertex_buffer = NULL; }

                if(blend_state) { blend_state->Release(); blend_state = NULL; }
                if(depth_stencil_state) { depth_stencil_state->Release(); depth_stencil_state = NULL; }
                if(rasterizer_state) { rasterizer_state->Release(); rasterizer_state = NULL; }
                if(input_layout) { input_layout->Release(); input_layout = NULL; }
            }
        }
    }
}