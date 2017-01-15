namespace euler579cs2
{
    class vectortriple
    {
        public vectortriple(vector u, vector v, vector n)
        {
            this.u = u;
            this.v = v;
            this.n = n;
        }

        public vector u { get; }
        public vector v { get; }
        public vector n { get; }
    }
}