#include <shell.h>
#include <syscall.h>
#define GREEN 49
#define WHITE 0x0F
#define DARK_GREY 46
void printLogo();


int main() {
     printLogo();
     shell();

     return 0;
}

void printLogo(){
changeFontColor(GREEN);
printf("\n\n");
printf("                                                                                                               \n");
printf("                                                                 dddddddd                                      \n");
printf("                               W E L C O M E    T O              d::::::d     OOOOOOOOO        SSSSSSSSSSSSSSS \n");
printf("                                                                 d::::::d   OO:::::::::OO    SS:::::::::::::::S\n");
printf("                                                                 d::::::d OO:::::::::::::OO S:::::SSSSSS::::::S\n");
printf("                                                                 d:::::d O:::::::OOO:::::::OS:::::S     SSSSSSS\n");
printf("                  nnnn  nnnnnnnn       ooooooooooo       ddddddddd:::::d O::::::O   O::::::OS:::::S            \n");
printf("                  n:::nn::::::::nn   oo:::::::::::oo   dd::::::::::::::d O:::::O     O:::::OS:::::S            \n");
printf("                  n::::::::::::::nn o:::::::::::::::o d::::::::::::::::d O:::::O     O:::::O S::::SSSS         \n");
printf("                  nn:::::::::::::::no:::::ooooo:::::od:::::::ddddd:::::d O:::::O     O:::::O  SS::::::SSSSS    \n");
printf("                    n:::::nnnn:::::no::::o     o::::od::::::d    d:::::d O:::::O     O:::::O    SSS::::::::SS  \n");
printf("                    n::::n    n::::no::::o     o::::od:::::d     d:::::d O:::::O     O:::::O       SSSSSS::::S \n");
printf("                    n::::n    n::::no::::o     o::::od:::::d     d:::::d O:::::O     O:::::O            S:::::S\n");
printf("                    n::::n    n::::no::::o     o::::od:::::d     d:::::d O::::::O   O::::::O            S:::::S\n");
printf("                    n::::n    n::::no:::::ooooo:::::od::::::ddddd::::::ddO:::::::OOO:::::::OSSSSSSS     S:::::S\n");
printf("                    n::::n    n::::no:::::::::::::::o d:::::::::::::::::d OO:::::::::::::OO S::::::SSSSSS:::::S\n");
printf("                    n::::n    n::::n oo:::::::::::oo   d:::::::::ddd::::d   OO:::::::::OO   S:::::::::::::::SS \n");
printf("                    nnnnnn    nnnnnn   ooooooooooo      ddddddddd   ddddd     OOOOOOOOO      SSSSSSSSSSSSSSS   \n");
printf("                                                                                                               \n");
printf("                                                                                                               \n");
changeFontColor(DARK_GREY);
printf("                                                                                                               \n");
printf("                                                                               I have a bad feeling about this.\n");
printf("                                                                                                               \n");
printf("                                                                                                               \n");
printf("                                                                                                               \n");
changeFontColor(WHITE);
}
