#include "deferlib.hpp"
#include <cstdio>
#include <cstdlib>

char *file_read_all_text(char const *filename)
{
    // The variables
    FILE *file;
    char *ans;
    size_t len;
    
    // Open the file
    file = fopen(filename, "rb");
    defer { // Guarantee that file will be cleaned up by the time we exit the current scope
        if(file != NULL)
            fclose(file);
    };
    if(file == NULL)
        return NULL;
    
    // Get the length of the contents of the file
    fseek(file, 0, SEEK_END);
    len = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Allocate a buffer for the contents that we're going to read
    ans = (char*)malloc(len + 1);
    if(ans == NULL)
        return NULL;
    
    // Read the contents of the file into the buffer
    fread(ans, sizeof(char), len, file);
    ans[len] = 0;
    
    return ans;
}

int file_display(char const *filename)
{
    char *str = file_read_all_text(filename);
    defer {
        if(str != NULL) free(str);
    }; // This is kinda retarded because we could just free AFTER use, but whatever, we're doing this for showcase sake!

    int ret = str == NULL ? -1 : 0; // We can actually write this inline within the return statement itself and it works just fine, the deferred code gets called AFTER the computation of the value to be returned, but before the actual ret instruction takes place, so it all works great! I just put it here so that we can use it on the printf call, that's all...

    printf("displaying \"%s\":\n\tvalid: %s\n\tstr: %s\n\tret: %d\n\n", filename, str == NULL ? "no" : "yes", str, ret);

    return ret;
}

int main()
{
    defer {
        printf("This should be executed at the end of the scope, no matter how we exit from here!\n");
    };
	
	defer noexcept {
        printf("We can also have noexcept defer blocks!\n");
    };

    file_display("deferlib.hpp");
    file_display("example.cpp");
    file_display("file_that_does_not_exist.txt");

    return 0;
}
