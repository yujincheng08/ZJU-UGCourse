import java.util.HashSet;
import java.util.ArrayList;
class Sodoku
{
    private int[][] matrix = new int[9][9];
    private boolean generated = false;
    private boolean[][] visible = new boolean[9][9];

    void print()
    {
        print(false);
    }

    void print(boolean visual)
    {
        for(int i = 0; i < 10; ++i)
        {
            if( i == 0)
                System.out.println("┌────┬────┬────┐");
            else if( i == 9 )
                System.out.println("└────┴────┴────┘");
            else if( i % 3 == 0 )
                System.out.println("├────┼────┼────┤");
            if(i < 9)
            {
                for(int j = 0; j < 10; ++j)
                {
                    if(j %3 == 0)
                        System.out.print('│');
                    if(j < 9)
                    {
                        if(j%3==1) System.out.print("  ");
                        System.out.print(visible[i][j] || visual ? matrix[i][j] : " ");
                        if(j%3==1) System.out.print("  ");
                    }
                }
            }
            System.out.println();
        }
    }
    private void clear()
    {
        for(int i = 0; i < 9; ++i)
            for(int j = 0; j < 9; ++j)
                matrix[i][j] = 0;
        generated = false;
    }

    private void produce()
    {
        ArrayList<ArrayList<HashSet<Integer>>> invalid = new ArrayList<>(9);
        int[][] available = new int[9][9];
        int[][] filled = new int[9][9];
        for(int i = 0; i < 9; ++i)
        {
            invalid.add(new ArrayList<>(9));
            for (int j = 0; j < 9; ++j)
            {
                invalid.get(i).add(new HashSet<>(9));
            }
        }
        if(generated)
            clear();
        for(int num = 1; num <= 9; ++num)
        {
            for(int block = 0; block < 9; ++block)
            {
                available[num-1][block] = 10-num;
                filled[num-1][block] = 0;
                invalid.get(num-1).get(block).clear();
            }
            blockLoop : for(int block = 0; block < 9; ++block)
            {
                HashSet<Integer> set = invalid.get(num-1).get(block);
                while(true)
                {
                    if (available[num-1][block] == 0) {
                        set.clear();
                        available[num-1][block] = 10 - num;
                        if(block==0)
                        {
                            --num;
                            block = 9;
                        }
                        --block;
                        --available[num-1][block];
                        int pos = filled[num-1][block];
                        //System.out.println(block + " " + pos);
                        invalid.get(num-1).get(block).add(pos);
                        int row = block / 3 * 3 + pos / 3;
                        int column = block % 3 * 3 + pos % 3;
                        matrix[row][column] = 0;
                        --block;
                        continue blockLoop;
                    }
                    int pos = (int) (Math.random() * 9);
                    if (!set.contains(pos))
                    {
                        int row = block / 3 * 3 + pos / 3;
                        int column = block % 3 * 3 + pos % 3;
                        //System.out.println(num + " " + row + " " + column + " " + block + " " + available[num-1][block]);
                        //print();
                        if (matrix[row][column] == 0)
                        {
                            if (check(num, row, column))
                            {
                                matrix[row][column] = num;
                                filled[num-1][block] = pos;
                                break;
                            }
                            else
                            {
                                --available[num-1][block];
                                set.add(pos);
                            }
                        }
                        else
                        {
                            set.add(pos);
                        }
                    }
                }
            }
        }
        generated = true;
    }

    private void hide(int hintNum)
    {
        while(hintNum>0)
        {
            int pos = (int)(Math.random()*81);

            if(!visible[pos/9][pos%9])
            {
                visible[pos / 9][pos % 9] = true;
                hintNum--;
            }
        }
    }

    void generate(int hintNum)
    {
        produce();
        hide(hintNum);
    }
    private boolean check(int num, int row, int column)
    {
        for(int i = 0; i<9; ++i)
            if(matrix[i][column] == num)
                return false;
        for(int j = 0; j<9; ++j)
            if(matrix[row][j] == num)
                return false;
        return true;
    }
}
