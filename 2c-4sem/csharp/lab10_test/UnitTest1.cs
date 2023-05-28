namespace lab10
{
    [TestClass]
    public class TOrganization
    {
        [TestMethod]
        public void GetFullInfo()
        {
            Organization org = new Organization("name", "address", "time");
            Assert.IsTrue(org.GetFullInfo() == "name, address, time");
        }
    }

    [TestClass]
    public class TFactory
    {
        [TestMethod]
        public void GetFullInfo()
        {
            Factory f = new Factory("name", "address", "time", 10, 10);
            Assert.IsTrue(f.GetFullInfo() == "Factory: name, address, time. (Done|Total) All(10|10)");
        }

        [TestMethod]
        public void getPlan()
        {
            Factory f = new Factory("name", "address", "time", 10, 10);
            Assert.AreEqual(10, f.getPlan());
        }

        [TestMethod]
        public void setPlan()
        {
            Factory f = new Factory("name", "address", "time", 10, 10);
            f.setPlan(10);
            Assert.AreEqual(10, f.getPlan());
        }

        [TestMethod]
        public void getDonePlan()
        {
            Factory f = new Factory("name", "address", "time", 10, 10);
            Assert.AreEqual(10, f.getDonePlan());
        }

        [TestMethod]
        public void setDonePlan()
        {
            Factory f = new Factory("name", "address", "time", 10, 10);
            f.setDonePlan(10);
            Assert.AreEqual(10, f.getDonePlan());
        }
    }

    [TestClass]
    public class TShipbuildingCompany
    {
        [TestMethod]
        public void GetFullInfo()
        {
            ShipbuildingCompany sc = new ShipbuildingCompany("name", "address", "time", 10, 10, 10, 10);
            Assert.AreEqual("ShipbuildingCompany: name, address, time (Total|Done) All(20|20) barges(10|10) riverbarges(10|10)", sc.GetFullInfo());
        }

        [TestMethod]
        public void GetBargesTotal()
        {
            ShipbuildingCompany sc = new ShipbuildingCompany("name", "address", "time", 10, 10, 10, 10);
            Assert.AreEqual(10, sc.GetBargesTotal());
        }

        [TestMethod]
        public void GetBargesDone()
        {
            ShipbuildingCompany sc = new ShipbuildingCompany("name", "address", "time", 10, 10, 10, 10);
            Assert.AreEqual(10, sc.GetBargesDone());
        }

        [TestMethod]
        public void GetRiverbargesTotal()
        {
            ShipbuildingCompany sc = new ShipbuildingCompany("name", "address", "time", 10, 10, 10, 10);
            Assert.AreEqual(10, sc.GetRiverbargesTotal());
        }

        [TestMethod]
        public void GetRiverbargesDone()
        {
            ShipbuildingCompany sc = new ShipbuildingCompany("name", "address", "time", 10, 10, 10, 10);
            Assert.AreEqual(10, sc.GetRiverbargesDone());
        }
    }


    [TestClass]
    public class TInsuranceCompany
    {
        [TestMethod]
        public void GetFullInfo()
        {
            Assert.IsTrue(new InsuranceCompany("name", "address", "time", 10).GetFullInfo() == "InsuranceCompany: name, address, time. Clients(10)");
        }

        [TestMethod]
        public void GetNumClients()
        {
            Assert.AreEqual(10, new InsuranceCompany("name", "address", "time", 10).GetNumClients());
        }
    }

    [TestClass]
    public class TLibrary
    {
        [TestMethod]
        public void GetFullInfo()
        {
            Assert.IsTrue(new Library("name", "address", "time", 10).GetFullInfo() == "Library: name, address, time. Number of books: 10");
        }

        [TestMethod]
        public void GetNumBooks()
        {
            Assert.AreEqual(10, new Library("name", "address", "time", 10).GetNumBooks());
        }

        [TestMethod]
        public void SetNumBooks()
        {
            Library lib = new Library("name", "address", "time", 10);
            lib.SetNumBooks(10);
            Assert.AreEqual(10, lib.GetNumBooks());
        }
    }
}