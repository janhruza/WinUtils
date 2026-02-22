using System.Text;
using System.Windows.Controls;
using System.Windows.Media;

namespace ThemeBuilder.Pages;

/// <summary>
/// Representing the colors page.
/// </summary>
public partial class PgColors : Page, IThemeBuilder
{
    public PgColors()
    {
        InitializeComponent();
        this.dColors = App.CreateThemeColors();
        RefreshUI();
    }

    public string ThemeSection => "[Control Panel\\Colors]";

    public string BuildThemeSection()
    {
        StringBuilder sb = new StringBuilder();
        _ = sb.AppendLine(ThemeSection);

        foreach (var pair in this.dColors)
        {
            string sValue = $"{pair.Value.R} {pair.Value.G} {pair.Value.B}";
            _ = sb.AppendLine($"{pair.Key}={sValue}");
        }

        return sb.ToString();
    }

    private Dictionary<string, Color> dColors;

    private void RefreshUI()
    {
        if (this.dColors == null) return;

        this.stp.Children.Clear();
        foreach (var kp in this.dColors)
        {
            Border bd = App.CreateColorPickerItem(this.dColors, kp.Key);
            _ = this.stp.Children.Add(bd);
        }

        return;
    }
}
