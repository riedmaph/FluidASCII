#include <stdio.h>    
#include <unistd.h>
#include <complex.h>  
//#define h      for(x=011; 2012-1>x++;)b[x]
//#define f(p,e) for(p=a; e,p<r; p+=5)
//#define z(e,i) f(p,p[i]=e)f(q,w=cabs(d=*p-*q)/2-1)if(0<(x=1-w))p[i]+=w*

// a: particle vector. each 5 complex numbers describe one particle:
// position (y, x), wall-flag, density, force, velocity
double complex a[97687], *p, *q, *r=a, w=0, d;
int x, y;
// b: output buffer, containing ANSI/VTxxx sequences:
// \x1b[2J    clear screen and move cursor to upper left
// \x1b[1;1H  move cursor to (1, 1)
char b[6856]="\x1b[2J\x1b[1;1H     ", *o=b, *t;

int main(){
	// process input
	for (;0<(x=getc(stdin));) {
		if (x>10) {
			if (32<x) {
				*r++ = w; 	// current partictle pos = w
				r[4] = w+1; // next partictle pos = w+1 (1 line below)
				*r=r[5]= x==35; // Wall flag (0 or 1) if input '#' for curr and next particle  (1 line below)
				r+=9;		// step to second next particle
			} else {
				// space-like whitespace
			}
			w = w - I; // decr x position
		} else {
			// linebreak-like whitespace
			x = w+2; // real part of w + 2, (x=0,y+=2), particle position = second next line
			w = x;
		}
	}

	// write output
	for (;;puts(o), o=b+4) { // print out buffer, clear screen only on first iteration
	  	// calculate densities
	  	for (p = a; p < r; p += 5) { // iterate over particles p
			p[2] = p[1] * 9; // init density with wall-flag * 9
			for (q = a; q < r; q += 5) { // iterate over particles q
				d = *p - *q;
				w = cabs(d) / 2 - 1; // (distance between p and q) / 2 - 1
				x = 1 - w;
				if (x > 0) { // w < 1 => adjacent
					p[2] += w * w; // increase density if adjacent particle found
				}
			}
		}
		p[2] = p[1] * 9; // probably not required, but didn't want to change semantics

	  	// calculate forces
		for (p = a; p < r; p += 5){ // iterate over particles p
			p[3] = G; // init force with gravity G
			for (q = a; q < r; q += 5) { // iterate over particles q
				d = *p - *q;
				w = cabs(d) / 2 - 1; // (distance between p and q) / 2 - 1
				x = 1 - w;
				if (x > 0) { // w < 1 => adjacent
					// calculate new force p[3] based on distance (w, d), density p[2], q[2], 
					// velocity p[4], viscosity V and pressure P
					// see smoothed-particle hydrodynamics (SPH)
					p[3] += w * (d * (3 - p[2] - q[2]) * P + p[4] * V - q[4] * V) / p[2];
				}
			}
		}
		p[3] = G; // probably not required, but didn't want to change semantics

		// clear out buffer
		for (x = 011; 2012 - 1 > x++;)
			b[x] = 0;
		
		// fill out buffer (out codes)
		for (p = a; p < r; p += 5) { // iterate over particles
			x = *p * I; // x position of particle
			y = *p / 2; // y position of particle
			t = b + 10 + x + 80 * y; // calc position in out buffer (line width 80)
			p[4] += p[3] / 10 * !p[1]; // new velocity depending on force (if not wall)
			*p += p[4]; // new position depending on velocity
			if (0<=x && x<79 && 0<=y && y<23) { // position within 80x24 window?
				// calculate out codes using marching squares algorithm:
				// bitwise OR with following pattern to get out codes 0..15:
				// 8 4   e.g. _ x  sums up to 5 wich is later 
				// 2 1        _ x  translated to '|'
				*t |= 8;
				t[1] |= 4;
				
				t += 80;  // next line in out buffer
				*t |=2;
				t[1] = 1;
				//x = *t; // not used
			} else {
				//x = 0; // not used
			}
		}
		*p += p[4] += p[3] / 10 * !p[1]; // probably not required, but didn't want to change semantics

		// translate out codes into nice characters and add linebreaks
		for (x = 011; 2012 - 1 > x++;)
			b[x] = " '`-.|//,\\|\\_\\/#\n"[x % 80 - 9 ? b[x] : 16];;

		// sleep 12ms before drawing next frame
		usleep(12321);
	}
	return 0;
}
