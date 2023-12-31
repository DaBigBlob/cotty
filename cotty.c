#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

typedef enum { false, true } bool;

void vprint(bool vopt, char* vtext) {
    if (vopt == true) printf("%s", vtext);
}

void print_license() {
    //i dont want to include license in the program but the license demands that i do TT
    printf(
        "[+] showing license text block\n\n"
        "BSD 3-Clause License\n"
        "\n"
        "Copyright (c) 2023, hman s. (localboxcrox@gmail.com) (github.com/DaBigBlob)\n"
        "\n"
        "Redistribution and use in source and binary forms, with or without\n"
        "modification, are permitted provided that the following conditions are met:\n"
        "\n"
        "1. Redistributions of source code must retain the above copyright notice, this\n"
        "   list of conditions and the following disclaimer.\n"
        "\n"
        "2. Redistributions in binary form must reproduce the above copyright notice,\n"
        "   this list of conditions and the following disclaimer in the documentation\n"
        "   and/or other materials provided with the distribution.\n"
        "\n"
        "3. Neither the name of the copyright holder nor the names of its\n"
        "   contributors may be used to endorse or promote products derived from\n"
        "   this software without specific prior written permission.\n"
        "\n"
        "THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\"\n"
        "AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n"
        "IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE\n"
        "DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE\n"
        "FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL\n"
        "DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR\n"
        "SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER\n"
        "CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,\n"
        "OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE\n"
        "OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n"
    );
}

void print_interactive_syntax() {
    printf(
        "\n"
        "INTERACTIVE MODE COMMANDS\n"
            "1\t\\v\tToggle verbosity.\n"
            "2\t\\c\tClear screen.\n"
            "3\t\\b\tSend backspace. (Will interactively ask for number of backspaces.)\n"
            "4\t\\h\tShow this help text block.\n"
            "5\t\\l\tShow the license.\n"
            "6\t\\s\tSend string. (Will interactively ask for string.)\n"
            "7\t\\n\tSend newline. Used as the ENTER/RETURN(Mac) key.\n"
            "8\t\\C\tSend Ctrl+c to slave TTY.\n"
            "9\t\\r\tRead whatever is on the slave TTY.\n"
            "10\t\\q\tExit interactive mode.\n"
            "11\t\tJust type and ENTER/RETURN(Mac) to send string. (Including newline.)\n"
        "\n"
    );
}

void print_syntax(char** argv) {
    printf(
        "[+] showing help text block\n\n"
        "NAME\n"
            "\tcotty - COntrol TTY\n"
        "\n"
        "SYNOPSIS\n"
            "\t%s -T path/to/tty [-v] [-d] [-c] [-b backspace-count] [-s string-to-send] [-n] [-h]\n"
        "\n"
        "DESCRIPTION\n"
            "\tControl input to a TTY.\n"
        "\n"
        "OPTIONS\n"
            "\t-T\tPath to the (slave) TTY you want to control.\n"
            "\t-v\tVerbose output.\n"
            "\t-d\tDisable interactive mode.\n"
            "\t-c\tClear Screen.\n"
            "\t-b\tNumber of backspaces to send to the TTY. Used as the BACKSPACE/DELETE(Mac) key.\n"
            "\t-s\tString to send.\n"
            "\t-n\tSend newline. Used as the ENTER/RETURN(Mac) key.\n"
            "\t-C\tSend Ctrl+c to slave TTY.\n"
            "\t-r\tRead whatever is on the slave TTY.\n"
            "\t-h\tShow this help text block and exit.\n"
            "\t-l\tPrint the license and exit.\n"
        "\n"
        "ORDER OF OPERATIONS\n"
        "The following is the order of operation in 1(one) run of cotty in (non-interactive mode) irrespective of order of options.\n"
            "\t1\t(-r)\tRead from slave TTY.\n"
            "\t2\t(-C)\tSend Ctrl+c.\n"
            "\t3\t(-b)\tSend backspaces.\n"
            "\t4\t(-c)\tClear Screen.\n"
            "\t5\t(-s)\tSend string.\n"
            "\t6\t(-n)\tSend newline character.\n"
            "\t7\t\tEnter interactive mode.\n"
        "\n"
        "BUGS\n"
            "\tDoes not work on MacOS due to restrictions on input buffer of another terminal.\n"
        "\n"
        "AUTHOR\n"
            "\thman s. <localboxcrox@gmail.com> <github.com/DaBigBlob>\n"
        , argv[0]
    );
}

int send_backspaces(int fd, unsigned int bopt, bool vopt) {
    int rt = 0;
    vprint(vopt, "[*] sending backspaces...");
    for (int i=0; i<bopt; i++) if (rt != -1) rt = ioctl(fd, TIOCSTI, "\b");
    if (rt == -1) {
        vprint(vopt, "\n");
        printf("[!] could not put all backspaces to the slave TTY\n");
        close(fd);
        return EXIT_FAILURE;
    } else {
        vprint(vopt, "done\n");
    }
    return EXIT_SUCCESS;
}

int clear_screen(int fd, bool vopt) {
    vprint(vopt, "[*] clearing screen...");
    if (write(fd, "\033[H\033[2J\033[3J", 11) == -1) {
        vprint(vopt, "\n");
        printf("[!] could not clear screen of slave TTY\n");
        close(fd);
        return EXIT_FAILURE;
    } else {
        vprint(vopt, "done\n");
    }
    return EXIT_SUCCESS;
}

int send_string(int fd, char* sopt, bool vopt) {
    if (sopt == NULL) return EXIT_FAILURE;
    int rt = 0;
    vprint(vopt, "[*] sending string...");
    for (int i=0; sopt[i]; i++) if (rt != -1) rt = ioctl(fd, TIOCSTI, sopt+i);
    if (rt == -1) {
        vprint(vopt, "\n");
        printf("[!] could not put all characters of string \"%s\" to the slave TTY\n", sopt);
        close(fd);
        return EXIT_FAILURE;
    } else {
        vprint(vopt, "done\n");
    }
    return EXIT_SUCCESS;
}

int send_newline(int fd, bool vopt) {
    vprint(vopt, "[*] sending newline...");
    if (ioctl(fd, TIOCSTI, "\n") == -1) {
        vprint(vopt, "\n");
        printf("[!] could not put newline to the slave TTY\n");
        close(fd);
        return EXIT_FAILURE;
    } else {
        vprint(vopt, "done\n");
    }
    return EXIT_SUCCESS;
}

int send_ctrl_c(int fd, bool vopt) {
    vprint(vopt, "[*] sending Ctrl+c...");
    if (ioctl(fd, TIOCSTI, "\x03") == -1) {
        vprint(vopt, "\n");
        printf("[!] could not send Ctrl+c to the slave TTY\n");
        close(fd);
        return EXIT_FAILURE;
    } else {
        vprint(vopt, "done\n");
    }
    return EXIT_SUCCESS;
}

void remove_newline(char* str, bool vopt) {
    vprint(vopt, "[*] removing newline...");
    while (*str != '\0') {
        if (*str == '\n') {
            *str = '\0';
            vprint(vopt, "done\n");
            return;
        }
        str++;
    }
}

int read_screen(int fd, bool vopt) {

    //DOES NOT WORK :{
    printf("[!] reading from slave TTY does not work right now\n");
    return EXIT_FAILURE;

    int available_bytes;
    vprint(vopt, "[*] reading slave TTY buffer size...");
    if (ioctl(fd, FIONREAD, &available_bytes) == -1) {
        vprint(vopt, "\n");
        printf("[!] could not read\n");
        return EXIT_FAILURE;
    } else {
        vprint(vopt, "done\n");
    }

    if (available_bytes < 1) {
        printf("[!] slave TTY buffer too small\n");
        return EXIT_FAILURE;
    }

    vprint(vopt, "[*] allocating buffer in memory...");
    char* buf = malloc(available_bytes);
    if (buf == NULL) {
        vprint(vopt, "\n");
        printf("[!] could not allocate\n");
        return EXIT_FAILURE;
    } else {
        vprint(vopt, "done\n");
    }
    
    vprint(vopt, "[*] reading slave TTY buffer...");
    ssize_t bytes_read = read(fd, buf, available_bytes);
    if (bytes_read <= 0) {
        vprint(vopt, "\n");
        printf("[!] could not read\n");
        if (buf != NULL) free(buf);
        return EXIT_FAILURE;
    } else {
        vprint(vopt, "done\n");
    }

    vprint(vopt, "[*] writing to output...");
    if(write(STDOUT_FILENO, buf, bytes_read) <= 0) {
        printf("[!] could not write\n");
        if (buf != NULL) free(buf);
        return EXIT_FAILURE;
    }

    if (buf != NULL) free(buf);
    return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
    //prelim total argument check
    if (
        (argc < 2) ||
        (argv[1][0] != '-')
    ) {
        printf("[!] wrong syntax\n");
        print_syntax(argv);
        return EXIT_FAILURE;
    }

    //TTY file descriptor
    int fd = 0;

    //option init stuff
    bool vopt = false;      //verbose
    bool dopt = false;      //disable interactive mode
    bool copt = false;      //clear screen
    unsigned int bopt = 0;  //backspace count
    char* sopt = NULL;      //string pointer
    bool nopt = false;      //newline
    bool Copt = false;      //ctrl + c
    bool ropt = false;      //read slave TTY
    int opt;

    //option parse
    while ((opt = getopt(argc, argv, ":T:vdcb:s:nChrl")) != -1) switch (opt) {
        case 'T' : {
            vprint(vopt, "[*] openning the slave TTY...");
            fd = open(optarg, O_RDWR);
            if(fd == -1) {
                printf("\n[!] could not open \"%s\"\n", optarg);
                print_syntax(argv);
                return EXIT_FAILURE;
            }
            vprint(vopt, "done\n");
            break;
        }
        case 'v' : {
            vopt = true;
            vprint(vopt, "[+] verbose mode set\n");
            break;
        }
        case 'd' : {
            dopt = true;
            vprint(vopt, "[-] interactive mode disabled\n");
            break;
        }
        case 'c' : {
            copt = true;
            vprint(vopt, "[+] clearing screen is scheduled\n");
            break;
        }
        case 'b' : {
            vprint(vopt, "[*] parcing backspace number...");
            bopt = atoi(optarg);
            if (bopt == 0) {
                printf("\n[!] argument provided for \"-b\" is not valid number\n");
                return EXIT_FAILURE;
            }
            vprint(vopt, "done\n");
            break;
        }
        case 's' : {
            sopt = optarg;
            vprint(vopt, "[+] setting string to be sent\n");
            break;
        }
        case 'n' : {
            nopt = true;
            vprint(vopt, "[+] scheduling newline character\n");
            break;
        }
        case 'C' : {
            Copt = true;
            vprint(vopt, "[+] scheduling Ctrl+c\n");
            break;
        }
        case 'h' : {
            print_syntax(argv);
            if (fd != 0) close(fd);
            return EXIT_SUCCESS;
        }
        case 'r' : {
            ropt = true;
            vprint(vopt, "[+] scheduling read from slave TTY\n");
            break;
        }
        case 'l' : {
            print_license();
            if (fd != 0) close(fd);
            return EXIT_SUCCESS;
        }
        case ':' : {
            printf("[!] argument of \"-%c\" not provided\n", optopt);
            print_syntax(argv);
            return EXIT_FAILURE;
        }
        default: {
            printf("[!] unknown option \"-%c\" provided\n", optopt);
            print_syntax(argv);
            return EXIT_FAILURE;
        }
    }

    //if no -T
    vprint(vopt, "[*] checking if slave TTY is set...");
    if (fd == 0) {
        vprint(vopt, "\n");
        printf("[!] compulsory field \"-T\" not set\n");
        print_syntax(argv);
        close(fd);
        return EXIT_FAILURE;
    }
    vprint(vopt, "ok\n");

    //check if TIOCSTI is supported
    vprint(vopt, "[*] checking if TTY buffer injection is possible...");
    if (ioctl(fd, TIOCSTI, "\0") == -1) {
        vprint(vopt, "\n");
        printf("[!] Operating System does not allow injecting into another TTY's input buffer\n");
        close(fd);
        return EXIT_FAILURE;
    }
    vprint(vopt, "ok\n");

    //if nothing to send
    vprint(vopt, "[*] checking if nothing to send...");
    if (
        (dopt == true) &&   //no interactive env
        (copt == false) &&  //no clear screen
        (bopt == 0) &&      //no backspace
        (sopt == NULL) &&   //no string to send
        (nopt == false) &&  //no newline char
        (Copt == false)
    ) {
        vprint(vopt, "\n");
        printf("[?] no fruitful work to be done\n");
        print_syntax(argv);
        close(fd);
        return EXIT_SUCCESS;
    }
    vprint(vopt, "ok\n");

    //send backspaces
    if (bopt > 0) send_backspaces(fd, bopt, vopt);

    //clear screen
    if (copt == true) clear_screen(fd, vopt);

    //send string (fn is NULL checked)
    send_string(fd, sopt, vopt);

    //send newline
    if (nopt == true) send_newline(fd, vopt);

    //send Ctrl+c
    if (Copt == true) send_ctrl_c(fd, vopt);

    //read from slave TTY
    if (ropt == true) read_screen(fd, vopt);

    //start interactive mode
    if (dopt == false) {
        vprint(vopt, "[+] entering interactive mode\n");
        char cmd[256];

        vprint(vopt, "[*] setting to ignore Ctrl+d...");
        struct termios term;
        if (tcgetattr(STDIN_FILENO, &term) == -1) {
            vprint(vopt, "\n");
            printf("[!] could not get terminal attributions\n");
            close(fd);
            return EXIT_FAILURE;
        }
        term.c_cc[VEOF] = 0;
        if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1) {
            vprint(vopt, "\n");
            printf("[!] could not set terminal attributions\n");
            close(fd);
            return EXIT_FAILURE;
        }
        vprint(vopt, "done\n");

        printf("\n=============================================\ncommand \"\\h\" for help\n");
        while (1) {
            printf("\nprompt> ");
            fgets(cmd, sizeof(cmd), stdin);

            if (cmd[0] == '\\') {
                vprint(vopt, "[+] entering command mode\n");

                switch (cmd[1]) {
                    case 'v' : {
                        if (vopt == true) {
                            printf("[-] turning verbose mode off\n");
                            vopt = false;
                        } else {
                            printf("[+] turning verbose mode on\n");
                            vopt = true;
                        }
                        break;
                    }
                    case 'c' : {
                        clear_screen(fd, vopt);
                        break;
                    }
                    case 'b' : {
                        printf("\nnumber of backspaces to send: ");
                        fgets(cmd, sizeof(cmd), stdin);

                        vprint(vopt, "[*] parcing backspace number...");
                        bopt = atoi(cmd);
                        if (bopt == 0) {
                            vprint(vopt, "\n");
                            printf("[!] not valid number\n");
                        } else {
                            vprint(vopt, "done\n");
                        }

                        //send backspaces
                        if (bopt > 0) send_backspaces(fd, bopt, vopt);
                        break;
                    }
                    case 'h' : {
                        print_interactive_syntax();
                        break;
                    }
                    case 'l' : {
                        print_license();
                        break;
                    }
                    case 's' : {
                        printf("\nstring to send: ");
                        fgets(cmd, sizeof(cmd), stdin);

                        remove_newline(cmd, vopt);
                        send_string(fd, cmd, vopt);
                        break;
                    }
                    case 'n' : {
                        send_newline(fd, vopt);
                        break;
                    }
                    case 'C' : {
                        send_ctrl_c(fd, vopt);
                        break;
                    }
                    case 'r' : {
                        read_screen(fd, vopt);
                        break;
                    }
                    case 'q' : {
                        printf("[*] quitting interactive mode\n");
                        close(fd);
                        return EXIT_SUCCESS;;
                    }
                    default: {
                        printf("[!] unknown command\n");
                        print_interactive_syntax();
                    }
                }
            } else {
                remove_newline(cmd, vopt);
                send_string(fd, cmd, vopt);
                send_newline(fd, vopt);
            }
        }
    }

    //final ok
    close(fd);
    return EXIT_SUCCESS;
}