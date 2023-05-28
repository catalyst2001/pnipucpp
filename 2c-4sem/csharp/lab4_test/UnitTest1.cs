using System;

namespace lab4
{
    [TestClass]
    public class UnitTest1
    {
        public bool CmpArrays(int[] arr1, int[] arr2)
        {
            if(arr1.Length != arr2.Length)
                return false;

            for(int i = 0; i < arr1.Length; i++)
                if (arr1[i] != arr2[i])
                    return false;

            return true;
        }

        public int[] CreateFilled(int n, bool b_inversefill = false)
        {
            int[] arr = new int[n];
            if(b_inversefill)
            {
                for (int i = arr.Length - 1; i > 0; i--)
                    arr[i] = i;

                return arr;
            }

            for(int i = 0; i < n; i++)
                arr[i] = i;

            return arr;
        }

        [TestMethod]
        public void RemoveFromArrayTest()
        {
            int[] array = CreateFilled(10);
            Program.RemoveFromArray(ref array, 0, 1);
            Assert.AreEqual(true, CmpArrays(array,
                new int[] { 1, 2, 3, 4, 5, 6, 7, 8, 9 })
            );
        }

        [TestMethod]
        public void InsertElemFrom_Test()
        {
            int[] array = CreateFilled(10);
            Program.InsertElemFrom(ref array, 0, 0);
            Assert.AreEqual(true, CmpArrays(array, 
                new int[] { 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 })
            );
        }

        [TestMethod]
        public void FindElemEqualsAverageElems_Test()
        {
            int avg = 0;
            int[] array = CreateFilled(10);
            Assert.AreEqual(4, Program.FindElemEqualsAverageElems(ref avg, array));
        }

        [TestMethod]
        public void SimpleSwapSort_Test()
        {
            int[] array = CreateFilled(10, true);
            Program.SimpleSwapSort(array);
            Assert.AreEqual(true, CmpArrays(array, new int[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));
        }
    }
}