using System;
using System.Windows.Forms;

namespace Projeto_Parcial_1
{
    public partial class GraphicsForm : Form
    {
        public GraphicsForm()
        {
            InitializeComponent();
        }
        GraphClass graph = new GraphClass();
        private void GraphicsForm_Load(object sender, EventArgs e)
        {
            chart1.Series[0].Points.Clear();
            chart2.Series[0].Points.Clear();
            int x = 0, X = 0;

            for (int i = 1; i <= graph.ldrIndex; i++)
            {
                chart1.Series[0].Points.AddXY(++x, graph.getldrValue(i));
            }

            for (int i = 1; i <= graph.potIndex; i++)
            {
                chart2.Series[0].Points.AddXY(++X, graph.getpotValue(i));
            }
        }
    }
}
