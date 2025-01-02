declare filename "noise.dsp";
declare name "noise";
import("stdfaust.lib");

L = no.noise;
R = no.pink_noise;

process = L , R;