using System.Media;
using System.Windows;
using System.Windows.Media;

namespace ThemeBuilder.Windows;

/// <summary>
/// Representing the custom color picker window.
/// </summary>
public partial class ColorPicker : Window
{
    public ColorPicker()
    {
        InitializeComponent();
        UpdatePreview();
    }

    public ColorPicker(string colorName)
    {
        InitializeComponent();
        Title += $" ({colorName})";
        UpdatePreview();
    }

    public ColorPicker(string colorName, Color cValue)
    {
        InitializeComponent();
        Title += $" ({colorName})";
        SetInitialColor(cValue);
        UpdatePreview();
    }

    private bool bDialog;
    private Color cResult;
    public Color Result => this.cResult;

    public new bool? ShowDialog()
    {
        Loaded += (s, e) =>
        {
            SystemSounds.Beep.Play();
        };

        this.bDialog = true;
        return base.ShowDialog();
    }

    private new void Show()
    {
        this.bDialog = false;
        base.Show();
        return;
    }

    private void btnClose_Click(object sender, RoutedEventArgs e)
    {
        if (this.bDialog)
        {
            DialogResult = false;
        }
    }

    private void btnOk_Click(object sender, RoutedEventArgs e)
    {
        if (this.bDialog)
        {
            DialogResult = true;
        }

        this.cResult = new Color();
        this.cResult.A = 255;
        this.cResult.R = Convert.ToByte(this.sRed.Value);
        this.cResult.G = Convert.ToByte(this.sGreen.Value);
        this.cResult.B = Convert.ToByte(this.sBlue.Value);
    }

    private void SetInitialColor(Color color)
    {
        this.sRed.Value = color.R;
        this.sGreen.Value = color.G;
        this.sBlue.Value = color.B;
        return;
    }

    private void UpdatePreview()
    {
        Color c = new Color();
        c.A = 255;
        c.R = Convert.ToByte(this.sRed.Value);
        c.G = Convert.ToByte(this.sGreen.Value);
        c.B = Convert.ToByte(this.sBlue.Value);
        this.bdPreview.Background = new SolidColorBrush(c);
        return;
    }

    private void sAll_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
    {
        UpdatePreview();
    }
}
