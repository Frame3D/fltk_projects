// generated by Fast Light User Interface Designer (fluid) version 1.0110

#include <libintl.h>
#include "flrun.h"
// (c) Robert Shingledecker 2008
// Command input search by Curaga 2011
#include <iostream>
#include <string>
#include <FL/Fl_File_Chooser.H>
#include <locale.h>
using namespace std;

static int matches(const char *param, char ***res, unsigned int *out) {
  char *origpath = getenv("PATH");
        char **dirs, *path, *ptr;

        if (!origpath) return 1;

        DIR *dir;
        struct dirent *ent;
        unsigned int i = 0, j = 1, apps = 0;

	for (i = 0;origpath[i]; i++) {
		if (origpath[i] == ':') j++;
	}
	dirs = (char **) calloc(j, sizeof(char *));

	path = strdup(origpath);
	char *save = path;
	for (i = 0; path; i++) {
		dirs[i] = strdup(path);
		ptr = strchr(dirs[i], ':');

		if (ptr) {
			*ptr = '\0';
			path = ptr + 1;
		} else path = NULL;
	}
	free(save);

        for (i = 0; i < j; i++) {
                dir = opendir(dirs[i]);
                if (!dir) continue;

                while ((ent = readdir(dir))) {
                        if (ent->d_type == DT_REG || ent->d_type == DT_LNK) apps++;
                }

                closedir(dir);
        }

        char **results = (char **) calloc(apps, sizeof(char *));
        if (!results) return 1;

        for (i = 0, apps = 0; i < j; i++) {
                dir = opendir(dirs[i]);
                if (!dir) continue;

                while ((ent = readdir(dir))) {
                        if (ent->d_type == DT_REG || ent->d_type == DT_LNK) {
                                if (strcasestr(ent->d_name, param)) {
                                        results[apps] = strdup(ent->d_name);
                                        apps++;
                                }
                        }
                }

                closedir(dir);
        }

        *res = results;
        *out = apps;

	for (i = 0; i < j; i++) free(dirs[i]);
	free(dirs);

        return 0;
}

static void btn_callback(Fl_Widget*, void* userdata) {
  if (userdata == "ok")
{
  string commandStr1 = in_command->value();
  string commandStr2 = " 2>/dev/null &";
  string command = commandStr1 + commandStr2;
  system(command.c_str());
  exit(0);
} else { 
    exit(0);
}
}

static void choose_cb(Fl_Widget*, void*) {
  Fl_File_Chooser chooser(".",                        // directory
                            "*",                        // filter
                            Fl_File_Chooser::SINGLE,     // chooser type
                            "Select Application to Run");  // title
    chooser.show();                                             

    // Block until user picks something.
    //     (The other way to do this is to use a callback())
    //                                                      
    while(chooser.shown())
        { Fl::wait(); }

    // User hit cancel?
    if ( chooser.value() == NULL )
       return; 
 
    in_command->value(chooser.value());
}

static int sort(const void *p1, const void *p2) {
  int i = strcmp(* (char * const *)p1, * (char * const *)p2);
if (i > 0) i = -1;
else if (i < 0) i = 1;

return i;
}

static void search_cb(Fl_Widget *, void *) {
  in_command->clear();

char **results;
unsigned int i;

if (matches(in_command->value(), &results, &i) != 0) return;

qsort(&results[0], i, sizeof (char *), sort);

if (i == 1) in_command->value(results[0]);

for (; i; i--) {
	in_command->add(results[i-1]);
	free(results[i-1]);
}
free(results);
}

Fl_Input_Choice *in_command=(Fl_Input_Choice *)0;

int main(int argc, char **argv) {
  Fl_Double_Window* w;
  setlocale(LC_ALL, "");
bindtextdomain("tinycore","/usr/local/share/locale");
textdomain("tinycore");
  { Fl_Double_Window* o = new Fl_Double_Window(260, 100, gettext("FLRun"));
    w = o;
    { Fl_Return_Button* o = new Fl_Return_Button(25, 65, 66, 20, gettext("OK"));
      o->callback((Fl_Callback*)btn_callback, (void*)("ok"));
    } // Fl_Return_Button* o
    { Fl_Button* o = new Fl_Button(100, 65, 64, 20, gettext("&Cancel"));
      o->callback((Fl_Callback*)btn_callback, (void*)("cancel"));
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(175, 65, 64, 20, gettext("Browse"));
      o->callback((Fl_Callback*)choose_cb);
    } // Fl_Button* o
    { in_command = new Fl_Input_Choice(25, 25, 215, 24, gettext("Application launcher"));
      in_command->callback((Fl_Callback*)search_cb);
      in_command->align(FL_ALIGN_TOP);
      in_command->when(FL_WHEN_CHANGED);
      in_command->take_focus();
    } // Fl_Input_Choice* in_command
    o->end();
  } // Fl_Double_Window* o
  w->show(argc, argv);
  return Fl::run();
}
