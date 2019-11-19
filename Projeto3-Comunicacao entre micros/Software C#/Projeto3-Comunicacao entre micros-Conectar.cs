using System;
using System.Windows.Forms;
using System.IO.Ports;

namespace Monitoramento_de_Sensores
{
    public partial class Conectar : Form
    {
        public Conectar()
        {
            InitializeComponent();
        }

        ClasseAux classeAux = new ClasseAux();
        Principal principal = new Principal();

        private void Form1_Load(object sender, EventArgs e)
        {
            timer1.Enabled = true;
        }

        private void Timer1_Tick(object sender, EventArgs e)
        {
            atualizaComs();
        }

        private void atualizaComs()
        {
            bool VariacaoComs = false;
            if (comboBox1.Items.Count != SerialPort.GetPortNames().Length)
            {
                VariacaoComs = true;
            }
            else
            {
                VariacaoComs = false;
            }
            if (VariacaoComs)
            {
                comboBox1.Items.Clear();
                foreach (string s in SerialPort.GetPortNames())
                {
                    comboBox1.Items.Add(s);
                }
            }
        }

        private void BtnConectar_Click(object sender, EventArgs e)
        {
            try
            {
                serialPort1.PortName = comboBox1.Items[comboBox1.SelectedIndex].ToString();
                serialPort1.Open();

                if (serialPort1.IsOpen)
                {
                    MessageBox.Show("Conexão bem-sucedida", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    classeAux.PortName = serialPort1.PortName;
                    serialPort1.Close();
                    timer1.Enabled = false;
                    this.Hide();
                    principal.ShowDialog();
                    this.Close();
                }
            }
            catch
            {
                MessageBox.Show("Conexão mal-sucedida", "Erro", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }
    }
}
