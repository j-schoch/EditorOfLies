#include "editor_header.h"
#include "editor_filemenu.h"
#include "editor_keyedit.h"
#include "editor_workspace.h"

#include <eol_dialog.h>
#include <eol_input.h>
#include <eol_logger.h>
#include <eol_graphics.h>
#include <eol_window.h>

typedef struct
{
  EditorLevelData *levelData;
}editorHeaderData;

/*local global variabls*/

/*local function prototypes*/
void editor_on_title_change(void *data)
{
  eolWindow *win;
  editorHeaderData *header;
  eolComponent *labelComp = NULL;
  if (!data)return;
  win = (eolWindow *)data;
  header = (editorHeaderData *)win->customData;
  if (!header)return;
  labelComp = eol_window_get_component_by_id(win,10);
  eol_label_set_text(labelComp,header->levelData->level->idName);
}

/*function definitions*/

eolBool editor_header_update(eolWindow *win,GList *updates)
{
  GList *c;
  eolLine title;
  eolComponent *comp = NULL;
  editorHeaderData *header = NULL;
  if ((win == NULL)||(updates == NULL))return eolFalse;
  header = (editorHeaderData *)win->customData;
  for (c = updates;c != NULL;c = c->next)
  {
    if (c->data == NULL)continue;
    comp = (eolComponent *)c->data;
    switch (comp->id)
    {
      case 0:
        editor_file_menu_window();
        return eolTrue;
      case 1:
        editor_key_edit_window(header->levelData->level->keys);
        return eolTrue;
    }
  }
  return eolFalse;
}

void editor_header_draw(eolWindow *win)
{
}

void editor_header_window(EditorLevelData *levelData)
{
  eolWindow *win;
  editorHeaderData * headerData;
  win = eol_window_load_from_file("ui/editor_header.def");
  if (win == NULL)return;
  win->update = editor_header_update;
  win->draw = editor_header_draw;
  win->customData = malloc(sizeof(editorHeaderData));
  memset(win->customData,0,sizeof(editorHeaderData));
  headerData = (editorHeaderData*)win->customData;
  headerData->levelData = levelData;
}


/*eol@eof*/
