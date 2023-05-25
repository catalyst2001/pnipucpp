using lab5;

namespace lab5_test
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public void FindElemEqualsAverageElems_Test()
        {
            int[] myArray = new int[10] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
            int average = 0;
            Assert.AreEqual(3, Program.FindElemEqualsAverageElems(ref average, myArray));
        }

        [TestMethod]
        public void ResizeMatrix_Test()
        {
            int[,] mat = new int[4, 4];
            for (int i = 0; i < mat.GetLength(0); i++)
                for (int j = 0; j < mat.GetLength(1); j++)
                    mat[i, j] = 1;

            Program.ResizeMatrix(ref mat, 5, 5);
            Assert.AreEqual(0, mat[0, 4]);
            Assert.AreEqual(0, mat[4, 4]);
            Assert.AreEqual(0, mat[4, 0]);
        }

        [TestMethod]
        public void FindRowsWithK_Test()
        {
            Random rn = new Random();
            int[][] raggArr = Program.GenRaggedArray(ref rn);
            for (int i = 0; i < raggArr.Length; i++)
            {
                for (int j = 0; j < raggArr[i].Length; j++)
                {
                    raggArr[i][j] = 1;
                }
            }

            ref int[] arr = ref raggArr[raggArr.Length - 1];
            arr[arr.Length - 1] = 0;
            Assert.AreEqual(raggArr.Length - 1, Program.FindRowsWithK(raggArr, 0));
        }
    }
}