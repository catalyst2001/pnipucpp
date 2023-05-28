using System;

namespace lab6
{
    [TestClass]
    public class UnitTest1
    {
        public bool CmpArrays(int[] arr1, int[] arr2)
        {
            if (arr1.Length != arr2.Length)
                return false;

            for (int i = 0; i < arr1.Length; i++)
                if (arr1[i] != arr2[i])
                    return false;

            return true;
        }

        public bool CmpArraysChar(char[] arr1, char[] arr2)
        {
            if (arr1.Length != arr2.Length)
                return false;

            for (int i = 0; i < arr1.Length; i++)
                if (arr1[i] != arr2[i])
                    return false;

            return true;
        }

        [TestMethod]
        public void IsVowel_Test()
        {
            Assert.IsFalse(Program.IsVowel('Y'));
        }

        [TestMethod]
        public void RemoveFromArray_Test()
        {
            int[] array = { 1, 2, 3 };
            Program.RemoveFromArray(ref array, 0, 1);
            Assert.IsTrue(CmpArrays(array, new int[] { 2, 3 }));
        }

        [TestMethod]
        public void RemoveLastVowel_Test()
        {
            char[] array = new char[] { '1', '1', 'a' };
            Program.RemoveLastVowel(ref array);
            Assert.IsTrue(CmpArraysChar(array, new char[] { '1', '1' }));
        }

        [TestMethod]

        public void StrChrs_Test()
        {
            string str = "1111!1111";
            Assert.AreEqual(4, Program.StrChrs(str, "!"));
        }

        [TestMethod]
        public void StrrChrs_Test()
        {
            string str = "11?11";
            Assert.AreEqual(2, Program.StrrChrs(str, "?!"));
        }

        [TestMethod]
        public void IsPrint_Test()
        {
            Assert.IsFalse(Program.IsPrint((char)2));
        }

        [TestMethod]
        public void SwapFirstAndLastSentence2_Test()
        {
            string src = "Hello.How are you?How your friend?";
            char[] arr = new char[src.Length];
            src.CopyTo(arr);

            string result = "How your friend?How are you?Hello.";
            Program.SwapFirstAndLastSentence2(ref arr);
            Assert.AreEqual(true, CmpArraysChar(arr, result.ToArray()));
        }
    }
}