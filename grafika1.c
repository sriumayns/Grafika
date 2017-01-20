#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>


int fbfd = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
long int screensize = 0;
char *fbp = 0;
int x = 0, y = 0;
long int location = 0;

//Prosedur
void printLetterU(int,int);
void printLetterI(int,int);
void putRGB(int,int,int,int);

int main(){    
    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    // Figure out the size of the screen in bytes
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
                    fbfd, 0);
    if ((int)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");

    x = 300; y = 100;       // Where we are going to put the pixel

    // Figure out where in memory to put the pixel
    for (y = 0; y < vinfo.yres-20; y++){
        for (x = 0; x < vinfo.xres; x++) {

            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                    (y+vinfo.yoffset) * finfo.line_length;

            if (vinfo.bits_per_pixel == 32) {
                *(fbp + location) = 100;        // Some blue
                *(fbp + location + 1) = 100;     // A little green
                *(fbp + location + 2) = 100;    // A lot of red
                *(fbp + location + 3) = 0;      // No transparency
            } else  { //assume 16bpp
                int b = 10;
                int g = (x-100)/6;     // A little green
                int r = 31-(y-100)/16;    // A lot of red
                unsigned short int t = r<<11 | g << 5 | b;
                *((unsigned short int*)(fbp + location)) = t;
            }

        }
    }

    //Print Huruf
    printLetterU(100,300);
    printLetterI(200,300);

    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}

void putRGB(int location, int r, int g, int b){
    *(fbp + location) = r;        // Some blue
    *(fbp + location + 1) = g;     // A little green
    *(fbp + location + 2) = b;    // A lot of red
    *(fbp + location + 3) = 0;      // No transparency
}

void printLetterI(int xx, int yy){
    for (y = yy; y < yy+80; y++){
        for (x = xx; x < xx+10; x++) {

            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                        (y+vinfo.yoffset) * finfo.line_length;

            if (vinfo.bits_per_pixel == 32) {
                putRGB(location,100,50,0);
            } 

        }

    }
}

void printLetterU(int xx, int yy){
    
    int xt = xx+10;
    for (y = yy; y < yy+70; y++){
        for (x = xt; x < xx+10; x++) {

            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                        (y+vinfo.yoffset) * finfo.line_length;

            if (vinfo.bits_per_pixel == 32) {
                putRGB(location,100,50,0);
            } 

        }
        if (xt>xx){
            xt--;
        }

    }

    int t =1;
    for (y = yy; y < yy+70; y++){
        for (x = xx+50; x < xx+t+50; x++) {

            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                    (y+vinfo.yoffset) * finfo.line_length;

            if (vinfo.bits_per_pixel == 32) {
                putRGB(location,100,50,0);
            } 

        }
        if (t<10){
            t++;
        }
    }

    xt = xx;
    t = 60;
    for (y = yy+70; y < yy+10+70; y++){
        for (x = xt; x < xt+t; x++) {

            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                    (y+vinfo.yoffset) * finfo.line_length;

            if (vinfo.bits_per_pixel == 32) {
                putRGB(location,100,50,0);
            } 

        }
        xt+=1;
        t -=2;
    }
}
