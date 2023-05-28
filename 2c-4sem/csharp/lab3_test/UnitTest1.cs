using lab3;

namespace lab3_test
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public void ComputeSN()
        {
            Assert.AreEqual(true, Math.Round(Math.Abs(Functions.ComputeSN(0.1, 10.0)) - 0.00499) < float.Epsilon);
        }

        [TestMethod]
        public void ComputeSE()
        {
            Assert.AreEqual(true, Math.Round(Math.Abs(Functions.ComputeSE(0.1, 0.0001)) - 0.00499) < float.Epsilon);
        }

        [TestMethod]
        public void ComputeY()
        {
            Assert.AreEqual(true, Math.Round(Math.Abs(Functions.ComputeY(0.1)) - 0.00499) < float.Epsilon);
        }
    }
}