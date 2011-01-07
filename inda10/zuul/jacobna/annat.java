/**
 *     public boolean check2DArray(int[][] A1, int[][] A2){
        int x = 0;
        int y = 0;
        int t = 0;
        for(int i=0; i<64;i++){
            if(x>=8){
                y+=1;
                x=0;
            }
            if(A1[x][y] == A2[x][y]){
                t+=1;
            } 
            x++;
        }
        if(t == 64)
                return true;
        return false;
    }
    
    public void print2Darray(int[][] A1){
        int x = 0;
        int y = 0;
        int t = 0;
        for(int i=0; i<50*50;i++){
            if(x>=50){
                y+=1;
                x=0;
                System.out.println();
            }
            System.out.print(A1[x][y]);
            x++;           
        }

    }
 */