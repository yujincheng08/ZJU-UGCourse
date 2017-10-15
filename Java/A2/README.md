# Sodoku

3150101155 余锦成

## 变量说明

1. `class Main`

   `int hintNum`: 提示数字数，由用户输入

   `Sodoku sodoku`: 数独对象，用于生成和显示数独。

2. `class Sodoku`

   1. `int[][] matrix`: 数独矩阵，用于储存数独里面的数字

   2. `boolean generated`: 布尔变量，表示数独是否被初始化过。用于生成时候判断是否重置`matrix`。

      ```java
      if(generated)
          clear();
      ```

   3. `boolean[][] visible`: 表示数独上的数字是否可见。可见即为提示数。可见数字个数和等于`hintNum`。

3. `void Sodoku::produce`

   1. `ValidList valid`: 是一个$9\times9\times9$的数组，用于第一维是数字，第二维是九宫格编号，第三维度是有效位置编号。如果某个数字在某个九宫格里面某个位置有效，则会在这个数组里面出现，否则不出现。
   2. `int[][] filled`: 是一个$9\times9$数组。维数意义同`valid`前两维一样。值代表上一次填充的位置。用于表示这个数字在这个九宫格里面最后一次填充的位置。用于回溯。

4. `void Sodoku::hide`

   `ArrayList<Integer> unhidden`: 用于表示还没有被隐藏的位置。用于随机生成`visible`。

## 实现思路

1. 出题思路

   生成数独题目的思路我采用的是挖空法。因为最后要输出时数独的答案，所以我先生成一个数独的完全解，然后进行挖空。

   ```java
   void generate(int hintNum)
   {
     produce();
     hide(hintNum);
   }
   ```

2. 挖空思路

   挖空用简单随机即可，因为Java自带的随机数分布是平均分布，所以挖空出来的效果会比较平均的。

   用一个数组来保存还没被隐藏的位置，这样保证随机数能命中未隐藏位置，可以用空间换来时间。

   ```java
   private void hide(int hintNum)
   {
     ArrayList<Integer> unhidden = new ArrayList<>(81);
     initialList(unhidden);
     while(hintNum-->0)
     {
       int pos = unhidden.get((int)(Math.random()*unhidden.size()));
       visible[pos / 9][pos % 9] = true;
       removeFromList(unhidden, pos);
     }
   }
   ```

3. 生成完全解思路

   生成一个数独的完全解是这个项目最困难的地方。因为数独的个数非常庞大，直接枚举肯定不可行。这里采用回溯算法剪枝以达到较高的效率。

   回溯也有好几个思路，从上到下从左到右随机填数是一个比较常见的思路。我这里采用另一种思路：由于每个数字在行列和九宫格里面只出现一次，所以我按照数字逐个填到九宫格里面。

   比如，我先在数独里面填入1：

   | 1    |      |      |      |      |      |      |      |      |
   | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
   |      |      |      |      |      | 1    |      |      |      |
   |      |      |      |      |      |      | 1    |      |      |
   |      |      | 1    |      |      |      |      |      |      |
   |      |      |      | 1    |      |      |      |      |      |
   |      |      |      |      |      |      |      |      | 1    |
   |      |      |      |      |      |      |      | 1    |      |
   |      | 1    |      |      |      |      |      |      |      |
   |      |      |      |      | 1    |      |      |      |      |

   然后再依次填入2、3、4等。

   但是填入过程中也可能会出现某个数字不可能填入，于是也是需要回溯的。我们记录每一个数字在每一个九宫格里面填入的最后一个位置到`int [][]filled`。有了这个位置，我们回溯的时候就能很简单地撤销了。同时我们维护一个可用位置的数组`ValidList valid`。这样我们随机挑选位置的时候只能从里面挑选（同`void Sodoku::hide`）。回溯的时候只需要简单地从有效位置数组中移除，就能实现剪枝。

   ```java
   //记录位置
   if (check(num, row, column))
   {
     matrix[row][column] = num;
     filled[num - 1][block] = pos;
     break;
   }
   //撤销操作
   if (noValidPosition())
   {
     valid.resetValidList(num, block);
     toLastPosition(num, block);
     int pos = filled[num - 1][block];
     removeFromList(valid, num, block, pos);
     int row = getRow(block, pos);
     int column = getColumn(block, pos);
     matrix[row][column] = 0;
     continue;
   }
   ```

   一直循环下去就可以生成一个完全解了。

## 进一步优化

上述过程中我们使用了空间换取时间。在本项目中，生成数独的个数比较少，实现这种优化其实就足够了。当然其实我们还有更多的优化空间，可以用于生成大量数独时候使用。我们针对的优化函数是`boolean check(int num, int row, int column)`。这个函数返回行列中这个数字是否有效（因为我们从数字和九宫格角度生成，所以不需要检查九宫格）。

观察函数原型：

```java
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
```

里面使用迭代检查行列中是否存在同样的数字。这种循环效率可能不尽人意。我们可以使用`HashSet<>`来提高查找效率。于是可以改写成：

```java
private boolean check(int num, int row, int column)
{
  return !this.column[column].contains(num)) &&
    !this.row[row].contains(num))
}
```

不过需要多维护$2\times9\times9$的空间来实现快速判断。 