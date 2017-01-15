using System.Collections.Generic;
using System.Linq;
using Helpers;

namespace euler579cs2
{
    public class abcd
    {
        public long a { get; }
        public long b { get; }
        public long c { get; }
        public long d { get; }

        public abcd(long a, long b, long c, long d)
        {
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }

        public vector[] get_vectors()
        {
            var numbers = new[] {a, b, c};
            var perms = Permutations.Of(numbers, 3, false, false);
            return perms.Select(p => new vector(p[0], p[1], p[2])).ToArray();
        }

        public abcd[] get_permutations()
        {
            long[] baseArray = {a, b, c};
            abcd[] perms = Permutations.Of(baseArray, 3, false, false).Select(ls => new abcd(ls[0], ls[1], ls[2], d)).ToArray();
            return perms;
        }
    }
}