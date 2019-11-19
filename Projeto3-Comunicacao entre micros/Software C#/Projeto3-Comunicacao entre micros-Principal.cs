using System;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace Monitoramento_de_Sensores
{
    public partial class Principal : Form
    {
        public Principal()
        {
            InitializeComponent();
        }

        ClasseAux classeAux = new ClasseAux();
        public string RxString;
        int temp, lum, xT = 0, xL = 0;

        //Arraste do Form
        [DllImport("user32.DLL", EntryPoint = "ReleaseCapture")]
        private extern static void ReleaseCapture();
        [DllImport("user32.DLL", EntryPoint = "SendMessage")]
        private extern static void SendMessage(IntPtr hwnd, int wmsg, int wparam, int lparam);

        private void BtnExit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void BtnMinimize_Click(object sender, EventArgs e)
        {
            WindowState = FormWindowState.Minimized;
        }

        private void PanelHead_MouseDown(object sender, MouseEventArgs e)
        {
            ReleaseCapture();
            SendMessage(Handle, 0x112, 0xf012, 0);
        }


        private void Principal_Load(object sender, EventArgs e)
        {
            lbCom.Text = classeAux.PortName;
            serialPort1.PortName = ClasseAux.portname;
            try
            {
                serialPort1.Open();
            }
            catch
            {
                MessageBox.Show("Erro durante conexão!", "Erro", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
        }

        private void TbSend_KeyDown(object sender, KeyEventArgs e)
        {
            if(e.KeyCode == Keys.Enter)
            {
                btnSend.PerformClick();
            }
        }

        private void BtnSend_Click(object sender, EventArgs e)
        {
            try
            {
                serialPort1.WriteLine('C' + tbSend.Text);
                tbChat.AppendText("Você->" + tbSend.Text + "\r\n");
                tbSend.Clear();
                tbChat.ScrollToCaret();
            }
            catch
            {
                MessageBox.Show("Verifica a conexão com o dispositivo!", "Erro de comunicação", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
        }

        private void BtnAtualizaTemp_Click(object sender, EventArgs e)
        {
            try
            {
                serialPort1.WriteLine("T");
            }
            catch
            {
                MessageBox.Show("Verifique a conexão com o dispositivo", "Erro de comunicação", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
        }

        private void BtnAtualizaLum_Click(object sender, EventArgs e)
        {
            try
            {
                serialPort1.WriteLine("L");
            }
            catch
            {
                MessageBox.Show("Verifique a conexão com o dispositivo", "Erro de comunicação", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
        }

        private void trata_dados(object sender, EventArgs e)
        {
            switch (RxString[0])
            {
                case 't':
                    temp = int.Parse(RxString.Substring(1, RxString.Length - 2));
                    lbTemp.Text = temp.ToString();
                    atualiza_grafico("Temp", temp);
                    break;
                case 'l':
                    lum = int.Parse(RxString.Substring(1, RxString.Length - 2));
                    lbLum.Text = lum.ToString();
                    atualiza_grafico("Lum",lum);
                    break;
                case 'C':
                    tbChat.AppendText("Friend->" + RxString.Substring(1, RxString.Length - 1) + '\n');
                    break;
            }
        }

        private void atualiza_grafico(string grafico, int value)
        {
            if (grafico == "Temp")
            {
                chartTemp.Series[0].Points.AddXY(++xT, value);
                if (value >= 20)
                {
                    MessageBox.Show("Dispositivo ultrapassou a temperatura máxima", "Atenção!", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    panelLum.Enabled = false;
                    panelChat.Enabled = false;
                    panelTemp.Enabled = false;
                }
            }
            else if (grafico == "Lum")
            {
                chartLum.Series[0].Points.AddXY(++xL, value);
            }
        }

        private void SerialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            RxString = serialPort1.ReadLine();
            Invoke(new EventHandler(trata_dados));
        }
    }
}