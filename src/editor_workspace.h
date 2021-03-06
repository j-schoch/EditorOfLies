#ifndef __EDITOR_WORKSPACE__
#define __EDITOR_WORKSPACE__
/*
Copyright 2012 EngineerOfLies
This file is part of the Engine of Lies game engine library and Editor of Lies
level editor.

The Engine of Lies (EOL) is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

EOL is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the EOL game engine and the EOL level editor.  If not,
see <http://www.gnu.org/licenses/>.
*/

#include <eol_window.h>
#include <eol_level.h>

/*working level data*/
typedef struct
{
  eolBool         updated; /**<if modified since last frame*/
  eolBool         modified;/**<if modified since last save*/
  eolBool         drawTileSelection;  /**<draw the selectedTile*/
  GList         * selectedTiles;      /**<list of tiles that are seleccted*/
  eolBool         drawMouseTile;      /**<if this is true, highlight the tile under the mouse*/
  eolVec3D        selectColor;        /**<color for items that are selected*/
  eolVec3D        mouseOverColor;     /**<color fow items under the mouse*/
  eolLine         path;
  eolLine         filename;
  eolLevel      * level;
  eolLevelLayer * activeLayer;
}EditorWorkspace;

/*workspace utility*/
/**
 * @brief closes current level and creates a new default level to edit
 * @param workspace the workspace where the level will be created
 */
void editor_workspace_create_new_level(eolWindow *workspace);

eolLevel * editor_workspace_get_level(eolWindow *workspace);

EditorWorkspace *editor_get_workspace(eolWindow *workspace);

eolBool editor_workspace_mouse_click(eolWindow *workspace);

/*layer operations*/

/**
 * @brief adds a new layer to the level
 * @param workspace the workspace to add a layer to
 */
void editor_workspace_add_layer(eolWindow *workspace);
void editor_workspace_delete_layer(eolWindow *workspace,eolUint layer);
void editor_workspace_select_layer(eolWindow *workspace,eolUint layer);
void editor_workspace_hide_layer(eolWindow *workspace, eolUint layer, eolBool hide);
void editor_workspace_move_layer_up(eolWindow *workspace,eolUint layer);
void editor_workspace_move_layer_down(eolWindow *workspace,eolUint layer);
void editor_workspace_rename_layer(eolWindow *workspace,eolUint layer,eolLine newName);

/**
 * @brief gets the layer by the index, or NULL if non existant
 */
eolLevelLayer *editor_workspace_get_layer(eolWindow *workspace,eolUint layerIndex);

/*background operations*/

eolBackground *editor_workspace_get_background(eolWindow *workspace,eolUint index);
void editor_workspace_delete_background(eolWindow *workspace,eolUint backgroundIndex);
void editor_workspace_add_background(eolWindow *workspace,eolLine modelFile);
void editor_workspace_hide_background(eolWindow *workspace,eolUint index,eolBool hide);
void editor_workspace_move_background_up(eolWindow *workspace,eolUint backgroundIndex);
void editor_workspace_move_background_down(eolWindow *workspace,eolUint backgroundIndex);

/*tile operations*/
/**
 * @brief add a tile to the tile map of the active layer.
 * NOTE: it will not replace a tile if one is already in the mouse position
 * @param workspace the workspace to add a tile too
 * @param tileIndex the index of the tile type to add
 */
void editor_workspace_add_mouse_tile(eolWindow *workspace,eolUint tileIndex);

/**
 * @brief get the tileType by its index from the active level
 * @param workspace the workspace to search
 * @param typeId the id of the tile type to seach for
 * @return NULL on error or not found, a pointer to the tile type otherwise
 */
eolTileType *editor_workspace_get_tile_by_id(eolWindow *workspace,eolUint typeId);

/**
 * @brief enable the highlighting of the tile under the mouse
 */
void editor_workspace_show_mouse_over_tile(eolWindow *workspace,eolBool show);


/*maintaining updates*/
eolBool editor_workspace_updated(eolWindow *workspace);
eolBool editor_workspace_modified(eolWindow *workspace);
void editor_workspace_clear_updated(eolWindow *workspace);

/**
 * @brief turn on or off highlighting of the tile the mouse is over
 * @param workspace set the option here
 * @param show if eolTrue, show the mouse tile, if eolFalse, don't
 */
void editor_workspace_show_mouse_over_tile(eolWindow *workspace,eolBool show);
void editor_workspace_select_tile(eolWindow *workspace,eolVec2D mousePosition);

/*Loading and saving the level*/
void editor_workspace_load_level(eolWindow *workspace,eolLine filename);
void editor_workspace_save_level(eolWindow *workspace);


/*constructor*/
eolWindow* editor_workspace();

#endif
