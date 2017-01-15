namespace euler579cs2
{
    public class cube
    {
        public vertex[] vertices { get; }

        private static readonly vertex O = new vertex(0,0,0);
        public cube(vector U, vector V, vector N, bool flipX, bool flipY, bool flipZ)
        {
            var tempvertices = new[]
            {
                O,
                U,
                V,
                N,
                U + V,
                U + N,
                V + N,
                U + V + N
            };
        }
    }
}