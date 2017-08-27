greedy() {
    int colorcount = 0;
    
    v[1] = 1;
    colorcount++;
    
    for (i = 2; i <= n; i++)
    {
        sea S el conjunto de colores;
        for j:= 1 to i-1 do
        {
            if (v j es adyacente a v i )agregue el color de v j a S;
        }
        
        k:=1;
        
        while (el color k está en S) do76
        {
            k:=k+1;
            asigne el color k a v i ;
        }
    }
}
