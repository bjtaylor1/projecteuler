using System;

namespace euler579cs2
{
    public class vertex : IEquatable<vertex>
    {
        public vertex(long x, long y, long z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public long x { get; }
        public long y { get; }
        public long z { get; }

        public bool Equals(vertex other)
        {
            if (ReferenceEquals(null, other)) return false;
            if (ReferenceEquals(this, other)) return true;
            return x == other.x && y == other.y && z == other.z;
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            if (ReferenceEquals(this, obj)) return true;
            if (obj.GetType() != this.GetType()) return false;
            return Equals((vertex)obj);
        }

        public override int GetHashCode()
        {
            unchecked
            {
                var hashCode = x.GetHashCode();
                hashCode = (hashCode * 397) ^ y.GetHashCode();
                hashCode = (hashCode * 397) ^ z.GetHashCode();
                return hashCode;
            }
        }

        public override string ToString()
        {
            return $"{x},{y},{z}";
        }

        public static bool operator ==(vertex left, vertex right)
        {
            return Equals(left, right);
        }

        public static bool operator !=(vertex left, vertex right)
        {
            return !Equals(left, right);
        }

        public static vertex operator +(vertex lhs, vertex rhs)
        {
            return new vertex(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
        }


    }
}