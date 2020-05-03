highest="30"
cmd="convert -resize 534x300 -delay 5 -loop 0 00.ppm 00.ppm 00.ppm {00..$highest}.ppm $highest.ppm $highest.ppm $highest.ppm {$highest..00}.ppm ani_opti.gif"
echo $cmd
