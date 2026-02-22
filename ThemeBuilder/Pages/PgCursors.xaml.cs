using System.Text;
using System.Windows.Controls;

namespace ThemeBuilder.Pages;

/// <summary>
/// Representing the cursors page.
/// </summary>
public partial class PgCursors : Page, IThemeBuilder
{
    public string ThemeSection => "[Control Panel\\Cursors]";

    public string BuildThemeSection()
    {
        StringBuilder sb = new StringBuilder();
        _ = sb.AppendLine(ThemeSection);

        foreach (var kp in this.dCursors)
        {
            _ = sb.AppendLine($"{kp.Key}={GetValue(kp.Key)}");
        }

        return sb.ToString();
    }

    public PgCursors()
    {
        InitializeComponent();
        this.dCursors = App.CreateThemeCursors();
        this.lWrappers = new List<InputWrapper>();

        for (int x = 0; x < this.dCursors.Count; x++)
        {
            this.lWrappers.Add(new InputWrapper());
        }

        RefreshUI();
    }

    private Dictionary<string, string> dCursors;
    private List<InputWrapper> lWrappers;

    private string GetValue(string sKey)
    {
        int index = this.dCursors.Keys.ToList().IndexOf(sKey);
        return this.lWrappers[index].Value;
    }

    private void RefreshUI()
    {
        if (this.dCursors == null) return;

        this.stp.Children.Clear();

        for (int x = 0; x < this.dCursors.Count; x++)
        {
            var kp = this.dCursors.ElementAt(x);
            Border bd = App.CreateOpenFileItem(kp.Key, this.lWrappers[x], "Cursors|*.cur;*.ani|Other|*.*", new System.Windows.Thickness(0, (x > 0 ? 5 : 0), 0, 0));
            _ = this.stp.Children.Add(bd);
        }
    }
}
