void clear_screen();

void main() {
  clear_screen();

  char* video_memory = (char*) 0xb8000;
  *video_memory = 'E';
  video_memory = video_memory + 2;
  *video_memory = 'R';
  video_memory = video_memory + 2;
  *video_memory = 'I';
  video_memory = video_memory + 2;
  *video_memory = 'C';
  video_memory = video_memory + 2;
  *video_memory = ' ';
  video_memory = video_memory + 2;
  *video_memory = ' ';
  
  // Another way of doing this
  char str[2] = "OS";
  *video_memory = str[0];
  video_memory = video_memory + 2;
  *video_memory = str[1];
}


void clear_screen() {
  char* start = (char*) 0xb8000;
  char* end = (char*) 0xb8000+0xfa0;
  while (start <= end) {
    *start = 0;
    start = start + 2;
  }
}

