set term png
set output "result.png"
plot 'result_1.txt' u 1:2 w lines, 'result_2.txt' u 1:2 w lines, 'result_3.txt' u 1:2 w lines, 
