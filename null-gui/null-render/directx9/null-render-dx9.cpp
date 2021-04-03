#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "null-render-dx9.h"

static LPDIRECT3DDEVICE9 dx9_device = NULL;
static LPDIRECT3DVERTEXBUFFER9 vertex_buffer = NULL;
static LPDIRECT3DINDEXBUFFER9 index_buffer = NULL;
static LPDIRECT3DTEXTURE9 font_texture = NULL;
static int vertex_buffer_size = 5000, index_buffer_size = 10000;

struct CUSTOMVERTEX {
    float    pos[3];
    D3DCOLOR clr;
    float    uv[2];
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

namespace null_render {
    namespace directx9 {
        bool init(IDirect3DDevice9* device) {
            settings::renderer_has_vtx_offset = true;

            dx9_device = device;
            dx9_device->AddRef();
            return true;
        }

        void begin() { 
            if (!font_texture) create_device_objects();
        }

        void shutdown() {
            invalidate_device_objects();
            if (dx9_device) {
                dx9_device->Release();
                dx9_device = NULL;
            }
        }

        void render_draw_data(helpers::draw_data* _draw_data) {
            if (_draw_data->display_size.x <= 0.0f || _draw_data->display_size.y <= 0.0f)
                return;

            if (!vertex_buffer || vertex_buffer_size < _draw_data->total_vtx_count) {
                if (vertex_buffer) { vertex_buffer->Release(); vertex_buffer = NULL; }
                vertex_buffer_size = _draw_data->total_vtx_count + 5000;
                if (dx9_device->CreateVertexBuffer(vertex_buffer_size * sizeof(CUSTOMVERTEX), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &vertex_buffer, NULL) < 0)
                    return;
            }

            if (!index_buffer || index_buffer_size < _draw_data->total_idx_count) {
                if (index_buffer) { index_buffer->Release(); index_buffer = NULL; }
                index_buffer_size = _draw_data->total_idx_count + 10000;
                if (dx9_device->CreateIndexBuffer(index_buffer_size * sizeof(unsigned short), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, sizeof(unsigned short) == 2 ? D3DFMT_INDEX16 : D3DFMT_INDEX32, D3DPOOL_DEFAULT, &index_buffer, NULL) < 0)
                    return;
            }

            IDirect3DStateBlock9* d3d9_state_block = NULL;
            if (dx9_device->CreateStateBlock(D3DSBT_ALL, &d3d9_state_block) < 0)
                return;

            D3DMATRIX last_world, last_view, last_projection;
            dx9_device->GetTransform(D3DTS_WORLD, &last_world);
            dx9_device->GetTransform(D3DTS_VIEW, &last_view);
            dx9_device->GetTransform(D3DTS_PROJECTION, &last_projection);

            CUSTOMVERTEX* vtx_dst;
            unsigned short* idx_dst;
            if (vertex_buffer->Lock(0, (UINT)(_draw_data->total_vtx_count * sizeof(CUSTOMVERTEX)), (void**)&vtx_dst, D3DLOCK_DISCARD) < 0) return;
            if (index_buffer->Lock(0, (UINT)(_draw_data->total_idx_count * sizeof(unsigned short)), (void**)&idx_dst, D3DLOCK_DISCARD) < 0) return;

            for (int n = 0; n < _draw_data->cmd_lists_count; n++) {
                const draw_list* cmd_list = _draw_data->cmd_lists[n];
                helpers::vertex* vtx_src = (null_render::helpers::vertex*)cmd_list->vtx_buffer.data();
                for (int i = 0; i < cmd_list->vtx_buffer.size(); i++) {
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
            dx9_device->SetStreamSource(0, vertex_buffer, 0, sizeof(CUSTOMVERTEX));
            dx9_device->SetIndices(index_buffer);
            dx9_device->SetFVF(D3DFVF_CUSTOMVERTEX);

            setup_render_state(_draw_data);

            int global_vtx_offset = 0;
            int global_idx_offset = 0;
            vec2 clip_off = _draw_data->display_pos;
            for (int n = 0; n < _draw_data->cmd_lists_count; n++) {
                const draw_list* cmd_list = _draw_data->cmd_lists[n];
                for (int cmd_i = 0; cmd_i < cmd_list->cmd_buffer.size(); cmd_i++) {
                    const helpers::cmd* pcmd = &cmd_list->cmd_buffer[cmd_i];

                    const RECT r = { (LONG)(pcmd->clip_rect.min.x - clip_off.x), (LONG)(pcmd->clip_rect.min.y - clip_off.y), (LONG)(pcmd->clip_rect.max.x - clip_off.x), (LONG)(pcmd->clip_rect.max.y - clip_off.y) };
                    const LPDIRECT3DTEXTURE9 texture = (LPDIRECT3DTEXTURE9)pcmd->texture_id;
                    dx9_device->SetTexture(0, texture);
                    dx9_device->SetScissorRect(&r);
                    dx9_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, pcmd->vtx_offset + global_vtx_offset, 0, (UINT)cmd_list->vtx_buffer.size(), pcmd->idx_offset + global_idx_offset, pcmd->elem_count / 3);
                }
                global_idx_offset += cmd_list->idx_buffer.size();
                global_vtx_offset += cmd_list->vtx_buffer.size();
            }

            dx9_device->SetTransform(D3DTS_WORLD, &last_world);
            dx9_device->SetTransform(D3DTS_VIEW, &last_view);
            dx9_device->SetTransform(D3DTS_PROJECTION, &last_projection);

            d3d9_state_block->Apply();
            d3d9_state_block->Release();
        }

        void setup_render_state(helpers::draw_data* _draw_data) {
            D3DVIEWPORT9 vp;
            vp.X = vp.Y = 0;
            vp.Width = (DWORD)_draw_data->display_size.x;
            vp.Height = (DWORD)_draw_data->display_size.y;
            vp.MinZ = 0.0f;
            vp.MaxZ = 1.0f;
            dx9_device->SetViewport(&vp);

            dx9_device->SetPixelShader(NULL);
            dx9_device->SetVertexShader(NULL);
            dx9_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
            dx9_device->SetRenderState(D3DRS_LIGHTING, FALSE);
            dx9_device->SetRenderState(D3DRS_ZENABLE, FALSE);
            dx9_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
            dx9_device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
            dx9_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
            dx9_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            dx9_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            dx9_device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
            dx9_device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
            dx9_device->SetRenderState(D3DRS_FOGENABLE, FALSE);
            dx9_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
            dx9_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
            dx9_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
            dx9_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
            dx9_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
            dx9_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
            dx9_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
            dx9_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

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
                dx9_device->SetTransform(D3DTS_WORLD, &mat_identity);
                dx9_device->SetTransform(D3DTS_VIEW, &mat_identity);
                dx9_device->SetTransform(D3DTS_PROJECTION, &mat_projection);
            }
        }

        bool create_fonts_texture() {
            unsigned char* pixels;
            int width, height, bytes_per_pixel;
            null_font::vars::font_atlas->get_tex_data_as_rgba32(&pixels, &width, &height, &bytes_per_pixel);

            font_texture = NULL;
            if (dx9_device->CreateTexture(width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &font_texture, NULL) < 0) return false;
            D3DLOCKED_RECT tex_locked_rect;
            if (font_texture->LockRect(0, &tex_locked_rect, NULL, 0) != D3D_OK) return false;
            for (int y = 0; y < height; y++) memcpy((unsigned char*)tex_locked_rect.pBits + tex_locked_rect.Pitch * y, pixels + (width * bytes_per_pixel) * y, (width * bytes_per_pixel));
            font_texture->UnlockRect(0);

            null_font::vars::font_atlas->tex_id = (void*)font_texture;

            return true;
        }

        bool create_device_objects() {
            if (!dx9_device) return false;
            if (!create_fonts_texture()) return false;
            return true;
        }

        void invalidate_device_objects() {
            if (!dx9_device) return;
            if (vertex_buffer) { vertex_buffer->Release(); vertex_buffer = NULL; }
            if (index_buffer) { index_buffer->Release(); index_buffer = NULL; }
            if (font_texture) { font_texture->Release(); font_texture = NULL; null_font::vars::font_atlas->tex_id = NULL; }
        }
    }
}
