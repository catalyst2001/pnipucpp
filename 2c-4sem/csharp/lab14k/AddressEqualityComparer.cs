
namespace lab14k
{
    internal class AddressEqualityComparer : IEqualityComparer<Organization>
    {
        public bool Equals(Organization x, Organization y)
        {
            // Сравниваем организации по их адресам
            return x.address == y.address;
        }

        public int GetHashCode(Organization obj)
        {
            // Возвращаем хэш-код на основе адреса
            return obj.address.GetHashCode();
        }
    }
}
