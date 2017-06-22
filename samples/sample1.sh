cat <<EOF | ../lyapunov -p |ppmtojpeg -q 95 > ${0%.*}.jpg
2.5*sin(x+a)^2+2
0-i*200
0
i*200
400
300
-1
1
-1
1
0
10
100
EOF
