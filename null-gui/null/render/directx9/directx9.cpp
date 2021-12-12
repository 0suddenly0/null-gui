#include "directx9.h"
#include "shaders/shaders.h"

static int vertex_buffer_size = 5000, index_buffer_size = 10000;

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

namespace null {
    namespace render {
        namespace directx9 {
            bool initialize(IDirect3DDevice9* _device) {
                device = _device;
                device->AddRef();
                return true;
            }

            void begin_frame() {
                shaders::begin_frame();
                if(!font_texture) create_device_objects();
            }

            void shutdown() {
                invalidate_device_objects();
                if(device) {
                    device->Release();
                    device = NULL;
                }
            }

            void render_draw_data(detail::draw_data* _draw_data) {
                if(_draw_data->display_size.x <= 0.0f || _draw_data->display_size.y <= 0.0f)
                    return;

                if(!vertex_buffer || vertex_buffer_size < _draw_data->total_vtx_count) {
                    if(vertex_buffer) { vertex_buffer->Release(); vertex_buffer = NULL; }
                    vertex_buffer_size = _draw_data->total_vtx_count + 5000;
                    if(device->CreateVertexBuffer(vertex_buffer_size * sizeof(vertex), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &vertex_buffer, NULL) < 0)
                        return;
                }

                if(!index_buffer || index_buffer_size < _draw_data->total_idx_count) {
                    if(index_buffer) { index_buffer->Release(); index_buffer = NULL; }
                    index_buffer_size = _draw_data->total_idx_count + 10000;
                    if(device->CreateIndexBuffer(index_buffer_size * sizeof(unsigned short), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, sizeof(unsigned short) == 2 ? D3DFMT_INDEX16 : D3DFMT_INDEX32, D3DPOOL_DEFAULT, &index_buffer, NULL) < 0)
                        return;
                }

                IDirect3DStateBlock9* d3d9_state_block = NULL;
                if(device->CreateStateBlock(D3DSBT_ALL, &d3d9_state_block) < 0)
                    return;

                D3DMATRIX last_world, last_view, last_projection;
                device->GetTransform(D3DTS_WORLD, &last_world);
                device->GetTransform(D3DTS_VIEW, &last_view);
                device->GetTransform(D3DTS_PROJECTION, &last_projection);

                vertex* vtx_dst;
                unsigned short* idx_dst;
                if(vertex_buffer->Lock(0, (UINT)(_draw_data->total_vtx_count * sizeof(vertex)), (void**)&vtx_dst, D3DLOCK_DISCARD) < 0) return;
                if(index_buffer->Lock(0, (UINT)(_draw_data->total_idx_count * sizeof(unsigned short)), (void**)&idx_dst, D3DLOCK_DISCARD) < 0) return;

                for(int n = 0; n < _draw_data->cmd_lists_count; n++) {
                    const draw_list* cmd_list = _draw_data->cmd_lists[n];
                    detail::vertex* vtx_src = (detail::vertex*)cmd_list->vtx_buffer.data();
                    for(int i = 0; i < cmd_list->vtx_buffer.size(); i++) {
                        vtx_dst->pos[0] = vtx_src->pos.x;
                        vtx_dst->pos[1] = vtx_src->pos.y;
                        vtx_dst->pos[2] = 0.0f;
                        unsigned int clr = vtx_src->clr.to_unsigned();
                        vtx_dst->clr = (clr & 0xFF00FF00) | ((clr & 0xFF0000) >> 16) | ((clr & 0xFF) << 16);
                        vtx_dst->uv[0] = vtx_src->uv.x;
                        vtx_dst->uv[1] = vtx_src->uv.y;
                        vtx_dst++;
                        vtx_src++;
                    }
                    memcpy(idx_dst, cmd_list->idx_buffer.data(), cmd_list->idx_buffer.size() * sizeof(unsigned short));
                    idx_dst += cmd_list->idx_buffer.size();
                }
                vertex_buffer->Unlock();
                index_buffer->Unlock();
                device->SetStreamSource(0, vertex_buffer, 0, sizeof(vertex));
                device->SetIndices(index_buffer);
                device->SetVertexDeclaration(vertex_declaration);
                device->SetFVF(D3DFVF_CUSTOMVERTEX);

                setup_render_state(_draw_data);

                int global_vtx_offset = 0;
                int global_idx_offset = 0;
                vec2 clip_off = _draw_data->display_pos;
                for(int n = 0; n < _draw_data->cmd_lists_count; n++) {
                    draw_list* cmd_list = _draw_data->cmd_lists[n];
                    for(int cmd_i = 0; cmd_i < cmd_list->cmd_buffer.size(); cmd_i++) {
                        detail::cmd* pcmd = &cmd_list->cmd_buffer[cmd_i];

                        if(pcmd->setup_render_state_update_call) {
                            setup_render_state(_draw_data);
                            continue;
                        }

                        if(pcmd->callback != nullptr) {
                            pcmd->callback(pcmd);
                            continue;
                        }

                        const RECT clip_rect = { (LONG)(pcmd->clip_rect.min.x - clip_off.x), (LONG)(pcmd->clip_rect.min.y - clip_off.y), (LONG)(pcmd->clip_rect.max.x - clip_off.x), (LONG)(pcmd->clip_rect.max.y - clip_off.y) };
                        const LPDIRECT3DTEXTURE9 texture = (LPDIRECT3DTEXTURE9)pcmd->texture_id;
                        device->SetTexture(0, texture);
                        device->SetScissorRect(&clip_rect);
                        device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, pcmd->vtx_offset + global_vtx_offset, 0, (UINT)cmd_list->vtx_buffer.size(), pcmd->idx_offset + global_idx_offset, pcmd->elem_count / 3);
                    }
                    global_idx_offset += cmd_list->idx_buffer.size();
                    global_vtx_offset += cmd_list->vtx_buffer.size();
                }

                device->SetTransform(D3DTS_WORLD, &last_world);
                device->SetTransform(D3DTS_VIEW, &last_view);
                device->SetTransform(D3DTS_PROJECTION, &last_projection);

                d3d9_state_block->Apply();
                d3d9_state_block->Release();
            }

            void setup_render_state(detail::draw_data* _draw_data) {
                D3DVIEWPORT9 vp;
                vp.X = vp.Y = 0;
                vp.Width = (DWORD)_draw_data->display_size.x;
                vp.Height = (DWORD)_draw_data->display_size.y;
                vp.MinZ = 0.0f;
                vp.MaxZ = 1.0f;
                device->SetViewport(&vp);

                shaders::setup_render_state(_draw_data);

                device->SetPixelShader(NULL);
                device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
                device->SetRenderState(D3DRS_LIGHTING, FALSE);
                device->SetRenderState(D3DRS_ZENABLE, FALSE);
                device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
                device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
                device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
                device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
                device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
                device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
                device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
                device->SetRenderState(D3DRS_FOGENABLE, FALSE);
                device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
                device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
                device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
                device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
                device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
                device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
                device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
                device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

                {
                    float L = _draw_data->display_pos.x + 0.5f;
                    float R = _draw_data->display_pos.x + _draw_data->display_size.x + 0.5f;
                    float T = _draw_data->display_pos.y + 0.5f;
                    float B = _draw_data->display_pos.y + _draw_data->display_size.y + 0.5f;
                    D3DMATRIX mat_identity = { { { 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f } } };
                    D3DMATRIX mat_projection =
                    { { {
                        2.0f / (R - L),   0.0f,         0.0f,  0.0f,
                        0.0f,         2.0f / (T - B),   0.0f,  0.0f,
                        0.0f,         0.0f,         0.5f,  0.0f,
                        (L + R) / (L - R),  (T + B) / (B - T),  0.5f,  1.0f
                    } } };
                    device->SetTransform(D3DTS_WORLD, &mat_identity);
                    device->SetTransform(D3DTS_VIEW, &mat_identity);
                    device->SetTransform(D3DTS_PROJECTION, &mat_projection);
                }
            }

            bool create_fonts_texture() {
                unsigned char* pixels;
                int width, height, bytes_per_pixel;
                font::font_atlas->get_tex_data_as_rgba32(&pixels, &width, &height, &bytes_per_pixel);

                font_texture = NULL;
                if(device->CreateTexture(width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &font_texture, NULL) < 0) return false;
                D3DLOCKED_RECT tex_locked_rect;
                if(font_texture->LockRect(0, &tex_locked_rect, NULL, 0) != D3D_OK) return false;
                for(int y = 0; y < height; y++) memcpy((unsigned char*)tex_locked_rect.pBits + tex_locked_rect.Pitch * y, pixels + (width * bytes_per_pixel) * y, (width * bytes_per_pixel));
                font_texture->UnlockRect(0);

                font::font_atlas->tex_id = (void*)font_texture;

                return true;
            }

            bool create_device_objects() {
                if(!device) return false;
                if(!create_fonts_texture()) return false;

                if(!vertex_declaration) {
                    constexpr D3DVERTEXELEMENT9 elements[]{
                        { 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
                        { 0, 8, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
                        { 0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
                        D3DDECL_END()
                    };
                    device->CreateVertexDeclaration(elements, &vertex_declaration);
                }

                shaders::create_shaders();

                return true;
            }

            void invalidate_device_objects() {
                if(!device) return;

                shaders::clear_shaders();

                if(vertex_buffer) { vertex_buffer->Release(); vertex_buffer = NULL; }
                if(index_buffer) { index_buffer->Release(); index_buffer = NULL; }
                if(vertex_declaration) { vertex_declaration->Release(); vertex_declaration = nullptr; }
                if(font_texture) { font_texture->Release(); font_texture = NULL; font::font_atlas->tex_id = NULL; }
            }

            void copy_texture(IDirect3DTexture9* source, IDirect3DTexture9* dest, rect source_region, rect dest_region) {
                if (source_region == 0.f) source_region = rect(vec2(0.f, 0.f), get_texture_size(source));
                if (dest_region == 0.f) dest_region = rect(vec2(0.f, 0.f), get_texture_size(dest));

                IDirect3DSurface9* source_surface = nullptr;
                source->GetSurfaceLevel(0, &source_surface);

                IDirect3DSurface9* dest_surface = nullptr;
                dest->GetSurfaceLevel(0, &dest_surface);

                RECT _rect_source = RECT{ (int)source_region.min.x, (int)source_region.min.y, (int)source_region.max.x, (int)source_region.max.y };
                RECT _rect_dest = RECT{ (int)dest_region.min.x, (int)dest_region.min.y, (int)dest_region.max.x, (int)dest_region.max.y };
                device->StretchRect(source_surface, &_rect_source, dest_surface, &_rect_dest, D3DTEXF_LINEAR);

                if (source_surface) source_surface->Release(); source_surface = nullptr;
                if (dest_surface) dest_surface->Release(); dest_surface = nullptr;
            }

            void backbuffer_to_texture(IDirect3DTexture9* texture, rect backbuffer_region, D3DTEXTUREFILTERTYPE filtering) {
                if (!texture) return;
                if (backbuffer_region.size().x == 0.f || backbuffer_region.size().y == 0.f) backbuffer_region = rect(vec2(0.f, 0.f), display_size);

                IDirect3DSurface9* back_buffer = nullptr;
                IDirect3DSurface9* surface = nullptr;
                if (device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &back_buffer) == D3D_OK) {
                    if (texture->GetSurfaceLevel(0, &surface) == D3D_OK) {
                        RECT backbuffer_rect = RECT{ (LONG)backbuffer_region.min.x, (LONG)backbuffer_region.min.y, (LONG)backbuffer_region.max.x, (LONG)backbuffer_region.max.y };
                        device->StretchRect(back_buffer, &backbuffer_rect, surface, nullptr, filtering);
                    }
                }

                if (back_buffer) { back_buffer->Release(); back_buffer = nullptr; }
                if (surface) { surface->Release(); surface = nullptr; }
            }
        }
    }
}