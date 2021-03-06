/**
* Project: VSXu: Realtime modular visual programming language, music/audio visualizer.
*
* This file is part of Vovoid VSXu.
*
* @author Jonatan Wallmander, Robert Wenzel, Vovoid Media Technologies AB Copyright (C) 2003-2013
* @see The GNU Public License (GPL)
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#ifdef _WIN32
#include <windows.h>
#endif
#include <vector>
#include "vsx_avector.h"
#include "vsx_string.h"
using namespace std;
#include "vsxfst.h"
#include <stdio.h>
#include <stdlib.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#ifdef _WIN32
#include <io.h>
#endif


char cur_path[4096];
vsx_string current_path = cur_path;

int main(int argc, char* argv[])
{
	vsx_string base_path = get_path_from_filename(vsx_string(argv[0]));

	printf("Vovoid VSX Zip\n");

  //char* ret_getcwd = getcwd(cur_path,4096);
	printf("current path is: %s\n", cur_path);

	//srand ( time(NULL)+rand() );

	if (argc == 1)
		printf(" * add \"-help\" for command syntax\n");
	//printf("\n\n");
  if (argc > 1)
  {
	  if (vsx_string(argv[1]) == "-help") {
			printf("VSXzip command line syntax:\n"
			 			 "-x [filename] (extract)\n");
			return 0;
	  }

		if (vsx_string(argv[1]) == "-x")
		{
			vsxf filesystem; // our master filesystem handler
			//printf("trying to load %s\n", argv[2]);
			filesystem.archive_load(argv[2]);
      if (filesystem.is_archive_populated()) {
        vsx_avector<vsxf_archive_info>* archive_files = filesystem.get_archive_files();
	    	for (unsigned long i = 0; i < (*archive_files).size(); ++i)
	    	{
	    		vsx_string out_filename = (*archive_files)[i].filename;
	    		vsx_string out_dir = get_path_from_filename(out_filename);
	    		vsx_string full_out_path = vsx_string(cur_path) + "/" + out_filename;
	    		create_directory( (char*) (vsx_string(cur_path) + "/" + out_dir).c_str() );
	    		printf("%s/%s\n", cur_path,  (*archive_files)[i].filename.c_str() );
	    		vsxf_handle* fpi = filesystem.f_open((*archive_files)[i].filename.c_str(), "r");
	    		if (fpi)
	    		{
	    			FILE* fpo = fopen(full_out_path.c_str(), "wb");
	    			if (fpo)
	    			{
	    				char* buf = filesystem.f_gets_entire(fpi);
	    				if (buf)
	    				{
	    					fwrite(buf,sizeof(char),fpi->size-1,fpo);
	    					free(buf);
	    				}
	    				fclose(fpo);
	    			}
	    			filesystem.f_close(fpi);
	    		}
	    	}
	    }
		}
  }
	return 0;
}
