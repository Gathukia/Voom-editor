#include "voom_main.h"

int render(){
   return 0; 
}
void free_cursor_positions(Status* info) {
  if (!info) {
    return;
  }
  free(info->cursor_line_position);
  free(info->cursor_home_position);
  free(info->cursor_end_position);
}

void free_status_info(Status* info) {
    if (!info) {
        return;
    }

    if (info->cursor_line_position) {
        free(info->cursor_line_position);
    }
    if (info->cursor_home_position) {
        free(info->cursor_home_position);
    }
    if (info->cursor_end_position) {
        free(info->cursor_end_position);
    }
    if (info->encoding) {
        free(info->encoding);
    }
    if (info->lang_server) {
        free(info->lang_server);
    }

    free(info);
}


Pos* create_pos(uint cur_y, uint cur_x) {
  Pos* pos = (Pos*)malloc(sizeof(Pos));
  if (!pos) {
    return NULL; // Handle memory allocation failure
  }
  pos->cur_y = cur_y;
  pos->cur_x = cur_x;
  return pos;
}
int fill_file_info(Status* info) {
    if(!info) {
        return EXIT_FAILURE; // Handle null pointer argument
    }

    info->cursor_line_position = create_pos(0, 0);
    info->cursor_home_position = create_pos(0, 0);
    info->cursor_end_position = create_pos(0, 0);
    if (!info->cursor_line_position || !info->cursor_home_position || !info->cursor_end_position) {
        free_status_info(info);
        return EXIT_FAILURE;
    }

    info->encoding = (char *)malloc(SMALL_BUF * sizeof(char));
    if (!info->encoding) {
        free_cursor_positions(info);
        return EXIT_FAILURE;
    }


    if ((cmd_info->supports_UTF8) == true)
        strncpy(info->encoding, "UTF-8", SMALL_BUF - 1);
    else 
        strncpy(info->encoding, "ASCCI", SMALL_BUF - 1);



    info->lang_server = (char *)malloc(SMALL_BUF * sizeof(char));

    return EXIT_SUCCESS;
}

Status* initialize_status() {
    Status* info = (Status*)malloc(sizeof(Status));
    if (!info) {
        return NULL; // Handle memory allocation failure
    }

    info->cursor_line_position = NULL;
    info->cursor_home_position = NULL;
    info->cursor_end_position = NULL;
    info->encoding = NULL;
    info->lang_server = NULL;

    return info;
}


int edit_files(Handle** file_handle) {
  if (cmd_info->resize_param) {
    render();
    cmd_info->resize_param = false;
    return EXIT_SUCCESS;
  }

  // Dereference argument to get the actual head of the linked list
  Handle* cur = *file_handle;
  Handle* default_handle = NULL;

  do {
    Handle* next = cur->next;

    cur->data.Status_info = initialize_status();
    if (!cur->data.Status_info) {
      free_status_info(cur->data.Status_info);
      return EXIT_FAILURE;
    }


    if (!fill_file_info(cur->data.Status_info)) {
        printf("Problems with buffer allocations\n");
        free_status_info(cur->data.Status_info);
        return EXIT_FAILURE;
    }

    if (strncpy(cur->data.Status_info->lang_server, choose_lang_server(cur->data.filename), SMALL_BUF - 1) != 0) {
        printf("Could not choose_lang_server\n");
        return EXIT_FAILURE;
    }


    // Check for default file
    if (cur->data.file_id == 0) {
      default_handle = cur;
    }

    cur = next;
  } while (cur != *file_handle);

  // Render the default file
  render(default_handle->data);

  return EXIT_SUCCESS;
}








// File Management:

//     open_file(filename): Opens a file and loads its content into the buffer.
//     save_file(filename): Saves the buffer content to a file.

// Cursor Movement:

//     move_up(amount): Moves the cursor up amount lines.
//     move_down(amount): Moves the cursor down amount lines.
//     move_left(amount): Moves the cursor left amount characters.
//     move_right(amount): Moves the cursor right amount characters.

// Text Insertion/Deletion:

//     insert_character(char): Inserts a character at the cursor position.
//     delete_character(): Deletes the character at the cursor position.

// Basic Editing Commands:

//     copy(start, end): Copies text from a range to a temporary buffer.
//     cut(start, end): Cuts text from a range and copies it to a temporary buffer.
//     paste(): I


    