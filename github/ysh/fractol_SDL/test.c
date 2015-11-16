/* xfractal.c by Adam Rogoyski (apoc@laker.net) Temperanc on EFNet irc
 * Copyright (C) 1998 Adam Rogoyski
 * A simple fractal generator for the Mandlebrot set and one other odd
 * fractal I got changing the mandlebrot and julia set functions.  You
 * can specify all the different paramaters of the fractal function on
 * the command line.  There are some different examples.  Use the mouse
 * to zoom in on any area.  Each fractal prints out it's command line
 * info so you can always generate it again (cool huh!).
 * do: ./xfractal --help for help.
 * compile: gcc -o xfractal xfractal.c -L/usr/X11R6/lib -lX11
 * or something like that for your system.  Uses only xlib.
 * I make some real bad assumptions about how many colors you have.
 * I don't claim to be a fractal expert.  I just think this is cool.
 * I will add the Julia set and other fractals when I get some time.
 * --- GNU General Public License Disclamer ---
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>


struct drawing
{
   int height;
   int width;
   int linewidth;
   GC gc;
   Pixmap pixmap;
};

struct window
{
   int screen;
   Display *display;
   Window win, root;
   unsigned long foreground;
   unsigned long background;
   XSizeHints hint;
   XWindowAttributes attribs;
   GC gc;
};


typedef struct
{
   double real;
   double imag;
}
complex;


#define MANDELBROT 1
#define JULIA      2

struct
{
   int type;
   int background;
   long depth;
   long color;
   double range;
   double max_distance;
   complex origin;
}
fractal_options;


#define COLORS   32768


void mandelbrot (struct drawing *, struct window *);
void julia (struct drawing *, struct window *);


void handleButtonPress    (XEvent *, struct drawing *, struct window *);
void handleExpose         (XEvent *, struct drawing *, struct window *);
void handleReparentNotify (XEvent *, struct window *);
void initialize           (struct drawing *, struct window *, int, char **);
void print_fractal_info   (void);


int
main (int argc, char **argv)
{
   int done = 0;
   struct drawing canvas;
   struct window win;
   XEvent myevent;

   initialize (&canvas, &win, argc, argv);
   if (fractal_options.type == MANDELBROT)
      mandelbrot (&canvas, &win);
   else
      julia (&canvas, &win);

   while (!done)
   {
      XNextEvent (win.display, &myevent);
      switch (myevent.type)
      {
         case ButtonPress:
              handleButtonPress (&myevent, &canvas, &win);
              break;
         case Expose:
              handleExpose (&myevent, &canvas, &win);
              break;
         case MappingNotify:
              XRefreshKeyboardMapping ((XMappingEvent *) &myevent);
              break;
         case ReparentNotify:
              handleReparentNotify (&myevent, &win);
              break;
         case KeyPress:
              exit (EXIT_SUCCESS);
              break;
      }
   }
   XFreeGC (win.display, win.gc);
   XDestroyWindow (win.display, win.win);
   XCloseDisplay (win.display);
   return EXIT_SUCCESS;
}



void
handleExpose (XEvent *myevent, struct drawing *canvas, struct window *win)
{
   if (myevent->xexpose.count == 0)
   {
      XCopyArea (myevent->xexpose.display, canvas->pixmap,
                myevent->xexpose.window, win->gc, 0, 0,
                canvas->width, canvas->height, 0, 0);
   }
}


void
handleReparentNotify (XEvent *myevent, struct window *win)
{
   XWindowAttributes attribs;

   XGetWindowAttributes (myevent->xreparent.display,
                         myevent->xreparent.parent, &attribs);
   XMoveResizeWindow (win->display, win->win, attribs.x, attribs.y,
          attribs.width - (2 * win->attribs.border_width),
          attribs.height - attribs.y - win->attribs.border_width);
}


void
initialize (struct drawing *canvas, struct window *win,
            int argc, char **argv)
{
   int i = 0;

   memset (canvas, 0, sizeof (struct drawing));
   memset (win, 0, sizeof (struct window));

   fractal_options.type = MANDELBROT;
   fractal_options.background = 0;
   fractal_options.depth = 20;
   fractal_options.range = 2.0;
   fractal_options.max_distance = 4.0;
   fractal_options.origin.real = -1.4;
   fractal_options.origin.imag = 1.0;
   fractal_options.color = 0x60;
   win->hint.height = 0;
   win->hint.width = 0;


   for (i = 1; i < argc; i++)
   {
      if (!strncmp (*(argv + i), "-?", 2)
       || !strncmp (*(argv + i), "--help", 6))
      {
         fprintf (stderr,
         "Simple fractal program by Adam Rogoyski (apoc@laker.net)\n" \
         "usage: %s [-type] [-d depth] [-r range] [-t max_distance]\n" \
         "          [-x origin.real] [-y origin.imag] [-bg] [-c color]\n" \
         "          [-width x] [-height y]\n" \
         "type:         -m for mandelbrot, -j for odd julia set\n" \
         "              type must be first or you will get defaults\n" \
         "bg:           Color convergent or divergent area\n" \
         "depth:        depth of fractal.  Defaults to 20 or 50\n" \
         "range:        defaults to 2.0 or 0.2\n" \
         "max_distance: defaults to 4.0 or 40.0\n" \
         "origin.real:  defaults to -1.4 or 1.3\n" \
         "origin.imag:  defaults to 1.0 or 0.1\n" \
         "color         color in hex, decimal, or octal.\n" \
         "width         width of image - defaults to full screen\n" \
         "height        height of image - defaults to full screen\n\n"
         "examples:\n" \
         "default mandelbrot: %s -m -d 20 -r 2.0 -t 4.0 -x -1.4 -y 1.0\n" \
         "default julia: %s -j -d 50 -r 0.2 -t 40.0 -x 1.3 -y 0.1\n" \
         "%s -j -d 50 -r 2.0 -t 4.0 -x 0.4 -y 1.0\n" \
         "%s -j -d 50 -r 2.0 -t 40.0 -x 0.4 -y 1.0\n" \
         "%s -j -d 50 -r 0.2 -t 4.0 -x 1.3 -y 0.1\n" \
         "%s -j -d 50 -r 0.2 -t 40.0 -x 1.3 -y 0.1\n" \
         "%s -m -bg -d 500\n" \
         "%s -m -d 20 -r 3.0 -t 4.0 -x -1.5 -y 1.45\n" \
         "%s -j -bg -d 500 -t 200.0\n" \
         "%s -m -d 20 -r 100.0 -t 4.0 -x -50 -y 50\n" \
"%s -m -d 10240 -c 96 -r 0.003906 -t 4.000000 -x -0.236699 -y -0.824674\n",
           *argv, *argv, *argv, *argv, *argv, *argv, *argv, *argv,
           *argv, *argv, *argv, *argv);
         exit (EXIT_SUCCESS);
      }
      else if (!strncmp (*(argv + i), "-m", 2))
      {
         fractal_options.type = MANDELBROT;
         fractal_options.depth = 20;
         fractal_options.range = 2.0;
         fractal_options.max_distance = 4.0;
         fractal_options.origin.real = -1.4;
         fractal_options.origin.imag = 1.0;
      }
      else if (!strncmp (*(argv + i), "-j", 2))
      {
         fractal_options.type = JULIA;
         fractal_options.depth = 50;
         fractal_options.range = 0.2;
         fractal_options.max_distance = 40.0;
         fractal_options.origin.real = 1.3;
         fractal_options.origin.imag = 0.1;
      }
      else if (!strncmp (*(argv + i), "-d", 2))
      {
         if (++i < argc)
            fractal_options.depth = (double) strtod (*(argv + i), NULL);
      }
      else if (!strncmp (*(argv + i), "-bg", 3))
      {
         fractal_options.background = 1;
      }
      else if (!strncmp (*(argv + i), "-r", 2))
      {
         if (++i < argc)
            fractal_options.range = (double) strtod (*(argv + i), NULL);
      }
      else if (!strncmp (*(argv + i), "-t", 2))
      {
         if (++i < argc)
            fractal_options.max_distance =
                               (double) strtod (*(argv + i), NULL);
      }
      else if (!strncmp (*(argv + i), "-x", 2))
      {
         if (++i < argc)
            fractal_options.origin.real
                             = (double) strtod (*(argv + i), NULL);
      }
      else if (!strncmp (*(argv + i), "-y", 2))
      {
         if (++i < argc)
            fractal_options.origin.imag
                             = (double) strtod (*(argv + i), NULL);
      }
      else if (!strncmp (*(argv + i), "-c", 2))
      {
         if (++i < argc)
            fractal_options.color = strtol (*(argv + i), NULL, 0);
      }
      else if (!strncmp (*(argv + i), "-width", 6))
      {
         if (++i < argc)
            win->hint.width = strtol (*(argv + i), NULL, 0);
      }
      else if (!strncmp (*(argv + i), "-height", 7))
      {
         if (++i < argc)
            win->hint.height = strtol (*(argv + i), NULL, 0);
      }
   }

   fprintf (stderr, "Adam Rogoyski's (apoc@laker.net) simple fractal program\n");
   fprintf (stderr, "%s --help for help\n", *argv);

   win->display = XOpenDisplay("");
   if (!win->display)
   {
      fprintf (stderr, "%s: Cannot open display\n", *argv);
      exit (EXIT_FAILURE);
   }
   win->screen = DefaultScreen (win->display);
   win->root = XDefaultRootWindow (win->display);
   XGetWindowAttributes (win->display, win->root, &(win->attribs));
   win->foreground = WhitePixel (win->display, win->screen);
   win->background = BlackPixel (win->display, win->screen);
   win->hint.x = 0;
   win->hint.y = 0;
   if (!win->hint.width)
      win->hint.width = win->attribs.width;
   if (!win->hint.height)
      win->hint.height = win->attribs.height;
   win->hint.flags = PPosition | PSize;
   win->win = XCreateSimpleWindow (win->display, DefaultRootWindow (win->display),
                win->hint.x, win->hint.y, win->hint.width, win->hint.height,
                5, win->foreground, win->background);
   XGetWindowAttributes (win->display, win->win, &(win->attribs));
   win->hint.width -= win->attribs.border_width;
   win->hint.height -= win->attribs.border_width;
   canvas->width  = win->hint.width;
   canvas->height = win->hint.height;
   win->hint.x += win->attribs.border_width;
   win->hint.y += win->attribs.border_width;
   XSetStandardProperties (win->display, win->win, *argv, *argv, None,
                           argv, argc, &(win->hint));
   win->gc = XCreateGC (win->display, win->win, 0, 0);
   canvas->gc = XCreateGC (win->display, win->win, 0, 0);

   XSetBackground (win->display, win->gc, win->background);
   XSetBackground (win->display, canvas->gc, win->foreground);
   XSetForeground (win->display, win->gc, win->foreground);
   XSetForeground (win->display, canvas->gc, win->background);

   XSelectInput (win->display, win->win,
                 KeyPressMask | ExposureMask | ButtonPressMask);
   canvas->pixmap = XCreatePixmap (win->display, win->win, canvas->width,
                                  canvas->height, win->attribs.depth);
   XMapRaised (win->display, win->win);
}



void
mandelbrot (struct drawing *canvas, struct window *win)
/* The famous Mandelbrot set, z = (z + k)^2 */
{
   int x, y;
   long  iteration, color;
   long depth = fractal_options.depth;
   double range = fractal_options.range;
   double max_distance = fractal_options.max_distance;
   double distance;
   complex origin;
   complex z, k, a;

   origin.real = fractal_options.origin.real;
   origin.imag = fractal_options.origin.imag;

   for (y = 0; y < canvas->height; y++)
      for (x = 0; x < canvas->width; x++)
      {
         /*  I hope my math is right here.  a, k, and z are complex numbers.
          *  k is the normalized (x,y) coordinate from the current origin.
          *  z = (z + k)^2
          *  a = z + k
          *  z = a * a
          *  z = (a.real^2 + a.imag^2) + (a.real * a.imag + a.real * a.imag)i
          *  z = (a.real^2 + a.imag^2) + 2 * (a.real * a.imag)i
          */
         int flag = 0;
         
         z.real = z.imag = 0.0;
         /* normalize */
         k.real = origin.real + (double) x / (double) canvas->width * range;
         k.imag = origin.imag - (double) y / (double) canvas->height * range;
         for (iteration = 0; iteration < depth; iteration++)
         {
            a.real = z.real + k.real;
            a.imag = z.imag + k.imag;
            z.real = (a.real * a.real) - (a.imag * a.imag);
            z.imag = 2 * a.real * a.imag;
            distance = z.real * z.real + z.imag * z.imag;
            if (distance >= max_distance)
            {
               flag = 1;
               if (fractal_options.background)
               {
                  break;
               }
               color = fractal_options.color + (int) distance % COLORS;
               XSetForeground (win->display, win->gc, color);
               XDrawPoint (win->display,
                     win->win, win->gc,
                     x, y);
               XDrawPoint (win->display,
                     canvas->pixmap, win->gc,
                     x, y);
               break;
            }
         }
         if (fractal_options.background && !flag)
         {
            XSetForeground (win->display, win->gc, 0x101010);
            XDrawPoint (win->display,
                  win->win, win->gc,
                  x, y);
            XDrawPoint (win->display,
                  canvas->pixmap, win->gc,
                  x, y);
         }
      }
}



void
julia (struct drawing *canvas, struct window *win)
/* so I used cut and paste for this function.  I'll change it
 * some other day.  Oh, this isn't the julia set.  Just
 * needed a name.
 */
{
   int x, y;
   long iteration, color;
   long depth = fractal_options.depth;
   double range = fractal_options.range;
   double max_distance = fractal_options.max_distance;
   double distance;
   complex origin;
   complex z, k, a;

   origin.real = fractal_options.origin.real;
   origin.imag = fractal_options.origin.imag;
   for (y = 0; y < canvas->height; y++)
      for (x = 0; x < canvas->width; x++)
      {
         /* Same deal here, except initialize z to the normalized (x,y)
          * coordinate.  k get's initialized to a random value I put in.
          */
         int flag = 0;
         k.real = -1.401155;
         k.imag = 0.0;
         z.real = origin.real + (double) x / (double) canvas->width * range;
         z.imag = origin.imag - (double) y / (double) canvas->height * range;
         for (iteration = 0; iteration < depth; iteration++)
         {
            a.real = z.real + k.real;
            a.imag = z.imag + k.imag;
            z.real = (a.real * a.real) - (a.imag * a.imag);
            z.imag = 2 * a.real * a.imag;
            distance = z.real * z.real + z.imag * z.imag;
            if (distance >= max_distance)
            {
               flag = 1;
               if (fractal_options.background)
               {
                  break;
               }
               color = fractal_options.color + (int) distance % COLORS;
               XSetForeground (win->display, win->gc, color);
               XSetBackground (win->display, win->gc, 0x00);
               XDrawPoint (win->display,
                     win->win, win->gc,
                     x, y);
               XDrawPoint (win->display,
                     canvas->pixmap, win->gc,
                     x, y);
               break;
            }
         }
         if (fractal_options.background && !flag)
         {
               XSetForeground (win->display, win->gc, 0x101010);
               XDrawPoint (win->display,
                     win->win, win->gc,
                     x, y);
               XDrawPoint (win->display,
                     canvas->pixmap, win->gc,
                     x, y);
         }
      }
   XSetForeground (win->display, win->gc, win->foreground);
}


void
handleButtonPress    (XEvent *myevent, struct drawing *canvas,
                      struct window *win)
/* Zoom in on the mouse pointer */
{
   double range = 0.0;


   range = fractal_options.range;

   /* center clicked area */
   fractal_options.origin.real += (double) (range * myevent->xbutton.x)
                                / (double) (canvas->width * 2.0);
   fractal_options.origin.imag -= (double) (range * myevent->xbutton.y)
                                / (double) (canvas->height * 2.0);

   /* and magnify */
   fractal_options.range /= 2.0;

   /* increase the depth so there's a picture worth looking at */
   fractal_options.depth *= 2.0;

   print_fractal_info ();

   XSetForeground (win->display, win->gc, 0x00);
   XSetBackground (win->display, win->gc, 0x00);
   XFillRectangle (win->display, canvas->pixmap, win->gc, 0, 0,
                   canvas->width, canvas->height);
   XClearWindow (win->display, win->win);
   if (fractal_options.type == MANDELBROT)
      mandelbrot (canvas, win);
   else
      julia (canvas, win);
}


void
print_fractal_info (void)
/* prints command line options to generate the current fractal */
{
   printf ("-%c ", (fractal_options.type == MANDELBROT) ? 'm' : 'j');
   if (fractal_options.background)
      printf ("-bg ");
   printf ("-d %ld -c %ld -r %f -t %f -x %f -y %f\n",
           fractal_options.depth,
           fractal_options.color,
           fractal_options.range,
           fractal_options.max_distance,
           fractal_options.origin.real,
           fractal_options.origin.imag);
}