import java.util.ArrayList;
import java.util.HashSet;

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

    private class ValidList extends  ArrayList<ArrayList<ArrayList<Integer>>>
    {
        ValidList()
        {
            for (int i = 1; i < 10; ++i)
            {
                ArrayList<ArrayList<Integer>> list = new ArrayList<>(9);
                add(list);
                for(int j = 0; j<9; ++j)
                {
                    list.add(new ArrayList<>(9));
                    resetValidList(i, j);
                }
            }
        }

        ArrayList<Integer> get(int num, int block)
        {
            return super.get(num-1).get(block);
        }

        void resetValidList(int num, int block)
        {
            ArrayList<Integer> list = get(num, block);
            for(int i = 0; i < 9; ++i)
                list.add(i);
        }

        void updateValidList(int num, int block, int [][] matrix)
        {
            resetValidList(num, block);
            ArrayList<Integer> list = get(num, block);
            for(int i = 0; i < 9; ++i)
            {
                int row = block / 3 * 3 + i / 3;
                int column = block % 3 * 3 + i % 3;
                if(matrix[row][column] != 0)
                    list.remove(new Integer(i));
            }

        }
    }

    private void removeFromList(ArrayList<Integer>list, int object)
    {
        list.remove(new Integer(object));
    }

    private void produce()
    {
        ValidList valid = new ValidList();
        int[][] filled = new int[9][9];
        if(generated)
            clear();
        for(int num = 1; num <= 9; ++num)
        {
            for(int block = 0; block < 9; ++block)
            {
                filled[num-1][block] = 0;
                valid.updateValidList(num, block, matrix);
            }
            blockLoop : for(int block = 0; block < 9; ++block)
            {
                ArrayList<Integer> list = valid.get(num,block);
                while(true)
                {
                    if (list.size() == 0)
                    {
                        valid.resetValidList(num, block);
                        if (block == 0)
                        {
                            --num;
                            block = 9;
                        }
                        --block;
                        int pos = filled[num - 1][block];
                        removeFromList(valid.get(num, block), pos);
                        //System.out.println(block + " " + pos);
                        int row = block / 3 * 3 + pos / 3;
                        int column = block % 3 * 3 + pos % 3;
                        matrix[row][column] = 0;
                        --block;
                        continue blockLoop;
                    }
                    int pos = list.get((int) (Math.random() * list.size()));
                    int row = block / 3 * 3 + pos / 3;
                    int column = block % 3 * 3 + pos % 3;
                    //System.out.println(num + " " + row + " " + column + " " + block + " " + available[num-1][block]);
                    //print();
                    if (matrix[row][column] == 0)
                    {
                        if (check(num, row, column))
                        {
                            matrix[row][column] = num;
                            filled[num - 1][block] = pos;
                            break;
                        } else
                        {
                            //--available[num-1][block];
                            //set.add(pos);
                            removeFromList(list, pos);
                        }
                    } else
                    {
                        removeFromList(list, pos);
                    }
                }
            }
        }
        generated = true;
    }

    private void hide(int hintNum)
    {
        ArrayList<Integer> unhidden = new ArrayList<>(81);
        for(int i = 0; i<81; ++i)
            unhidden.add(i);
        while(hintNum>0)
        {
            int pos = unhidden.get((int)(Math.random()*unhidden.size()));
            visible[pos / 9][pos % 9] = true;
            removeFromList(unhidden, pos);
            hintNum--;
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
