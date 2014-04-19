#include <vgui/IVGui.h>
#include "ienginevgui.h"
#include "cdll_int.h"
#include "filesystem.h"
#include "bitmap/tgaloader.h"
#include "vtf/vtf.h"
#include "materialsystem/itexture.h"

#pragma once

class EstrangedMenuUtil
{
	public:
		static vgui::VPANEL GetChildByName(vgui::VPANEL parent, const char* name)
		{
			for (int i = 0; i < vgui::ipanel()->GetChildCount(parent); i++)
			{
				vgui::VPANEL child = vgui::ipanel()->GetChild(parent, i);

				if (Q_strcmp(name, vgui::ipanel()->GetName(child)) == 0)
				{
					return child;
				}
			}

			return NULL;
		}

		static void CloseGameFrames(vgui::VPANEL from)
		{
			vgui::VPANEL BaseGameUI = GetBaseGameUIPanel();

			if (!BaseGameUI) return;

			vgui::VPANEL LoadGameDialog = GetChildByName(BaseGameUI, "LoadGameDialog");
			vgui::VPANEL SaveGameDialog = GetChildByName(BaseGameUI, "SaveGameDialog");
			vgui::VPANEL OptionsDialog = GetChildByName(BaseGameUI, "OptionsDialog");

			KeyValues* closeCommand = new KeyValues("command", "command", "Close");
			if (LoadGameDialog)	vgui::ivgui()->PostMessage(LoadGameDialog, closeCommand, from);
			if (SaveGameDialog)	vgui::ivgui()->PostMessage(SaveGameDialog, closeCommand, from);
			if (OptionsDialog)	vgui::ivgui()->PostMessage(OptionsDialog, closeCommand, from);

			engine->ClientCmd("ae_showoptionspanel 0");
		}

		static bool IsGameUIVisible()
		{
			return enginevgui->IsGameUIVisible();
		}

		static vgui::VPANEL GetBaseGameUIPanel()
		{
			vgui::VPANEL root = enginevgui->GetPanel(PANEL_ROOT);
			if (!root) return NULL;

			vgui::VPANEL GameUI = GetChildByName(root, "GameUI Panel");
			if (!GameUI) return NULL;

			return GetChildByName(GameUI, "BaseGameUIPanel");
		}

		static bool ConvertITextureToVTF(ITexture *pItexture, const char *pDestVTF)
		{
			/*int indexImageSize;

			indexImageSize = ImageLoader::GetMemRequired(pItexture->GetActualWidth(), pItexture->GetActualHeight(), 1, pItexture->GetImageFormat(), false);

			unsigned char *pImage = (unsigned char*)new unsigned char[indexImageSize];

			size_t bytes;
			void const *test = pItexture->GetResourceData(VTF_RSRC_SHEET, &bytes);
			if (test)
			{
				DevMsg("valid pointer\n");
			}
			else
			{
				DevMsg("invalid pointer\n");
			}

			return true;

			pImage = (unsigned char*)pItexture->GetResourceData(0, (size_t*)indexImageSize);

			IVTFTexture *vtf = CreateVTFTexture();
			vtf->Init(pItexture->GetActualWidth(), pItexture->GetActualHeight(), pItexture->GetMappingDepth(), pItexture->GetImageFormat(), pItexture->GetFlags(), 1);
			unsigned char* pDest = vtf->ImageData(0, 0, 0);

			memcpy(pDest, pImage, indexImageSize);

			// Write
			CUtlBuffer buffer;
			vtf->Serialize(buffer);
			bool vtfWriteSuccess = g_pFullFileSystem->WriteFile(pDestVTF, "MOD", buffer);

			// Clean up
			DestroyVTFTexture(vtf);
			buffer.Clear();*/

			return true;
		}

		// Converts the source TGA to the destination VTF on disk. Returns true on success.
		// VTF flags are members of the enum CompiledVtfFlags in vtf.h.
		// Pass the optional argument as true to remove the source TGA after a successful conversion.
		static bool ConvertTGAtoVTF(const char *pSourceTGA, const char *pDestVTF, int pVTFFlags, bool pRemoveSourceTGA = false)
		{
			enum ImageFormat indexImageFormat;
			int indexImageSize;
			float gamma;
			int w, h;

			if (!TGALoader::GetInfo(pSourceTGA, &w, &h, &indexImageFormat, &gamma))
			{
				Warning("Unable to find TGA: %s\n", pSourceTGA);
				return false;
			}

			indexImageSize = ImageLoader::GetMemRequired(w, h, 1, indexImageFormat, false);

			unsigned char *pImage = (unsigned char*)new unsigned char[indexImageSize];

			if (!TGALoader::Load(pImage, pSourceTGA, w, h, indexImageFormat, gamma, false))
			{
				Warning("Unable to load TGA: %s\n", pSourceTGA);
				return false;
			}

			// Create a new VTF
			IVTFTexture *vtf = CreateVTFTexture();

			// Initialise it with some flags (nomipmap is good for using in the VGUI)
			vtf->Init(w, h, 1, indexImageFormat, pVTFFlags, 1);
			unsigned char* pDest = vtf->ImageData(0, 0, 0);

			// Memcpy it to the VTF
			memcpy(pDest, pImage, indexImageSize);

			// Write
			CUtlBuffer buffer;
			vtf->Serialize(buffer);
			bool vtfWriteSuccess = g_pFullFileSystem->WriteFile(pDestVTF, "MOD", buffer);

			// Clean up
			DestroyVTFTexture(vtf);
			buffer.Clear();

			if (!vtfWriteSuccess)
			{
				Warning("Unable to write VTF: %s\n", pDestVTF);
				return false;
			}

			// Remove the original
			if (pRemoveSourceTGA)
			{
				g_pFullFileSystem->RemoveFile(pSourceTGA, "MOD");
			}

			return true;
		}
};