using System.Windows;
using System.Windows.Media;

namespace ThemeBuilder;

/// <summary>
/// Representing the main window class.
/// </summary>
public partial class MainWindow : Window
{
    public MainWindow()
    {
        this._colors = App.CreateThemeColors();
        InitializeComponent();
        LoadPages();
    }

    private void LoadPages()
    {
        if (this.frmGeneral != null)
        {
            this.frmGeneral.Content = App.pgGeneral;
        }

        if (this.frmColors != null)
        {
            this.frmColors.Content = App.pgColors;
        }

        if (this.frmCursors != null)
        {
            this.frmCursors.Content = App.pgCursors;
        }
    }

    private Dictionary<string, Color> _colors;

    private void btnClose_Click(object sender, RoutedEventArgs e)
    {
        Close();
    }

    private void btnCreate_Click(object sender, RoutedEventArgs e)
    {
        // create theme file
        return;
    }
}