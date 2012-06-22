#include "eol_component.h"
#include "eol_logger.h"

/*local global variables*/

/*local function prototypes*/
void eol_component_button_new(eolComponent *component);
void eol_component_label_new(eolComponent *component);
void eol_component_label_free(eolComponent *component);

void eol_component_textinput_new(eolComponent *component);
void eol_component_textinput_get_text(eolComponent *component,char *text);
eolComponentInput *eol_component_textinput_get_component(eolComponent *component);

void eol_component_actor_new(eolComponent *component);
void eol_component_actor_load(eolComponent *component,char * filename);

eolComponentButton *eol_component_get_button_data(eolComponent *component);

void eol_component_button_free(eolComponent *component);
void eol_component_label_free(eolComponent *component);
void eol_component_textinput_free(eolComponent *component);
void eol_component_actor_free(eolComponent *component);
void eol_component_slider_free(eolComponent *component);
void eol_component_list_free(eolComponent *component);
void eol_component_image_free(eolComponent *component);

/*definitions*/

eolComponent * eol_component_new()
{
  eolComponent *component = NULL;
  component = (eolComponent *)malloc(sizeof(eolComponent));
  if (component == NULL)
  {
    eol_logger_message(
      EOL_LOG_ERROR,
      "eol_component: failed to allocate new component!\n");
    return NULL;
  }
  memset(component,0,sizeof(eolComponent));
  return component;
}

void eol_component_free(eolComponent **component)
{
  if ((!component)||(!*component))return;
  if ((*component)->data_free != NULL)
  {
    (*component)->data_free(*component);
  }
  free(*component);
  *component = NULL;
}
void eol_component_update(eolComponent *component);
void eol_component_set_focus(eolComponent *component,eolBool focus);
eolBool eol_component_has_changed(eolComponent *component);
eolInt eol_component_get_state(eolComponent *component);

eolComponentButton *eol_component_get_button_data(eolComponent *component)
{
  if ((!component)||
    (!component->componentData)||
    (component->type != eolButtonComponent))
  {
    return NULL;
  }
  return (eolComponentButton*)component->componentData;
}

eolComponentList *eol_component_get_list_data(eolComponent *component)
{
  if ((!component)||
    (!component->componentData)||
    (component->type != eolListComponent))
  {
    return NULL;
  }
  return (eolComponentList*)component->componentData;
}

eolComponentLabel *eol_component_get_label_data(eolComponent *component)
{
  if ((!component)||
    (!component->componentData)||
    (component->type != eolLabelComponent))
  {
    return NULL;
  }
  return (eolComponentLabel*)component->componentData;
}

eolComponentInput *eol_component_get_input_data(eolComponent *component)
{
  if ((!component)||
    (!component->componentData)||
    (component->type != eolInputComponent))
  {
    return NULL;
  }
  return (eolComponentInput*)component->componentData;
}

eolComponentImage *eol_component_get_image_data(eolComponent *component)
{
  if ((!component)||
    (!component->componentData)||
    (component->type != eolImageComponent))
  {
    return NULL;
  }
  return (eolComponentImage*)component->componentData;
}

eolComponentActor *eol_component_get_actor_data(eolComponent *component)
{
  if ((!component)||
    (!component->componentData)||
    (component->type != eolActorComponent))
  {
    return NULL;
  }
  return (eolComponentActor*)component->componentData;
}

eolComponentSlider *eol_component_get_slider_data(eolComponent *component)
{
  if ((!component)||
    (!component->componentData)||
    (component->type != eolSliderComponent))
  {
    return NULL;
  }
  return (eolComponentSlider*)component->componentData;
}


void eol_component_button_free(eolComponent *component)
{
  eolComponentButton *button;
  int i;
  button = eol_component_get_button_data(component);
  if (button)return;
  for (i = 0; i < eolButtonStateMax; i++)
  {
    if (button->button[i] != NULL)
    {
      eol_sprite_free(&button->button[i]);
    }
  }
  free(button);
  button = NULL;
  component->componentData = NULL;
}

void eol_component_label_free(eolComponent *component)
{
  eolComponentLabel *label = eol_component_get_label_data(component);
  if (label == NULL)return;
  if (label->buffer != NULL)free(label->buffer);
  if (label->font != NULL)eol_font_free(&label->font);
  free(label);
  label = NULL;
  component->componentData = NULL;
}

void eol_component_textinput_free(eolComponent *component)
{
  eolComponentInput *input = eol_component_get_input_data(component);
  if (input == NULL)return;
  if (input->buffer != NULL)free(input->buffer);
  if (input->font != NULL)eol_font_free(&input->font);
  free(input);
  input = NULL;
  component->componentData = NULL;
}

void eol_component_actor_free(eolComponent *component)
{
  eolComponentActor *actor = eol_component_get_actor_data(component);
  if (actor == NULL)return;
  eol_actor_free(&actor->actor);
  free(actor);
  component->componentData = NULL;
}

void eol_component_slider_free(eolComponent *component)
{
  eolComponentSlider *slider = eol_component_get_slider_data(component);
  if (slider == NULL)return;
  eol_sprite_free(&slider->slider);
  eol_sprite_free(&slider->bar);
  free(slider);
  component->componentData = NULL;
}

void eol_component_list_free(eolComponent *component)
{
  GList *l;
  eolComponentList * list = eol_component_get_list_data(component);
  if (list == NULL)return;
  l = list->itemList;
  while (l != NULL)
  {
    eol_component_free((eolComponent**)&l->data);
  }
  free(list);
  g_list_free(list->itemList);
  list->itemList = NULL;
  free(list);
  component->componentData = NULL;
}

void eol_component_image_free(eolComponent *component)
{
  eolComponentImage * image = eol_component_get_image_data(component);
  if (image == NULL)return;
  eol_sprite_free(&image->image);
  free(image);
  component->componentData = NULL;
}

void eol_component_label_draw(eolComponent *component, eolRect bounds)
{
  eolComponentLabel *label = eol_component_get_label_data(component);
  if (label == NULL)return;
  if (label->buffer == NULL)return;
  if (label->font == NULL)
  {
    if (label->wordWrap)
    {
      eol_font_draw_text_block(
      label->buffer,
      bounds.x,
      bounds.y,
      bounds.w,
      bounds.h,
      label->color,
      label->alpha,
      label->fontSize
      );
    }
    else
    {
      eol_font_draw_text_justify(
        label->buffer,
        bounds.x,
        bounds.y,
        label->color,
        label->alpha,
        label->fontSize,
        label->justify
      );
    }
  }
  else
  {
    if (label->wordWrap)
    {
      eol_font_draw_text_block_custom(
        label->buffer,
        bounds,
        label->color,
        label->alpha,
        label->font
      );
    }
    else
    {
      eol_font_draw_text_justify_custom(
        label->buffer,
        bounds.x,
        bounds.y,
        label->color,
        label->alpha,
        label->font,
        label->justify
      );
    }
  }
}

void eol_component_draw(eolComponent *component,eolRect bounds)
{
  if (!component)return;
  if (component->data_draw == NULL)return;
  eolRect drawRect;
  drawRect.x = bounds.x + (bounds.w * component->rect.x);
  drawRect.y = bounds.y + (bounds.h * component->rect.y);
  drawRect.w = (bounds.w * component->rect.w);
  drawRect.h = (bounds.h * component->rect.h);
  component->data_draw(component,drawRect);
}

void eol_component_make_label(
    eolComponent * component,
    char         * text,
    eolInt         fontSize,
    char         * fontName,
    eolVec3D       color,
    eolFloat       alpha
  )
{
  eolComponentLabel * label = NULL;
  if (!component)return;
  eol_component_label_new(component);
  label = eol_component_get_label_data(component);
  label->buffer = strdup(text);
  if (label->buffer == NULL)
  {
    eol_logger_message(
      EOL_LOG_ERROR,
      "eol_component:Failed to duplicate string for new label\n");
    eol_component_label_free(component);
    return;
  }
  if ((fontName != NULL)&&(strlen(fontName) > 0))
  {
    label->font = eol_font_load(fontName,fontSize);
  }
  label->fontSize = fontSize;
  label->alpha = alpha;
  eol_vec3d_copy(label->color,color);
  component->data_free = eol_component_label_free;
  component->data_draw = eol_component_label_draw;
}

void eol_component_label_new(eolComponent *component)
{
  if (component->componentData != NULL)
  {
    eol_logger_message(
      EOL_LOG_WARN,
      "eol_component:tried to make a label out of an existing component\n");
    return;
  }
  component->componentData = malloc(sizeof(eolComponentLabel));
  if (component->componentData == NULL)
  {
    eol_logger_message(
      EOL_LOG_ERROR,
      "eol_actor: failed to allocate data for new label\n");
    return;
  }
  memset(component->componentData,0,sizeof(eolComponentLabel));
}
/*eol@eof*/

