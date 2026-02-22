using System.Text;
using System.Windows;
using System.Windows.Controls;

namespace ThemeBuilder.Pages;

/// <summary>
/// Representing the general theme page.
/// </summary>
public partial class PgGeneral : Page, IThemeBuilder
{
    // inherited data
    public string ThemeSection => "[Theme]";
    public string BuildThemeSection()
    {
        StringBuilder sb = new StringBuilder();
        _ = sb.AppendLine(ThemeSection);
        _ = sb.AppendLine($"DisplayName={sName}");
        _ = sb.AppendLine($"BrandImage={sImage}");
        _ = sb.AppendLine($"[CLSID\\{{20D04FE0-3AEA-1069-A2D8-08002B30309D}}\\DefaultIcon]");
        _ = sb.AppendLine($"DefaultValue={sIconComputer}");
        _ = sb.AppendLine($"[CLSID\\{{59031A47-3F72-44A7-89C5-5595FE6B30EE}}\\DefaultIcon]");
        _ = sb.AppendLine($"DefaultValue={sIconDocuments}");
        _ = sb.AppendLine($"[CLSID\\{{F02C1A0D-BE21-4350-88B0-7367FC96EF3C}}\\DefaultIcon]");
        _ = sb.AppendLine($"DefaultValue={sIconNetwork}");
        _ = sb.AppendLine($"[CLSID\\{{645FF040-5081-101B-9F08-00AA002F954E}}\\DefaultIcon]");
        _ = sb.AppendLine($"Full={sIconRecycleBinFull}");
        _ = sb.AppendLine($"Empty={sIconRecycleBinEmpty}");
        return sb.ToString();
    }

    public PgGeneral()
    {
        InitializeComponent();
        RefreshUI();
    }

    // wrapper values
    private InputWrapper sNameW = new InputWrapper();
    private InputWrapper sImageW = new InputWrapper();
    private InputWrapper sIconComputerW = new InputWrapper();
    private InputWrapper sIconDocumentsW = new InputWrapper();
    private InputWrapper sIconNetworkW = new InputWrapper();
    private InputWrapper sIconRecycleBinFullW = new InputWrapper();
    private InputWrapper sIconRecycleBinEmptyW = new InputWrapper();

    // field values
    public string sName => this.sNameW.Value;
    public string sImage => this.sImageW.Value;
    public string sIconComputer => this.sIconComputerW.Value;
    public string sIconDocuments => this.sIconDocumentsW.Value;
    public string sIconNetwork => this.sIconNetworkW.Value;
    public string sIconRecycleBinFull => this.sIconRecycleBinFullW.Value;
    public string sIconRecycleBinEmpty => this.sIconRecycleBinEmptyW.Value;

    private const string sIconFilter = "Icon Files|*.ico|Other|*.*";

    private void RefreshUI()
    {
        this.stp.Children.Clear();

        _ = this.stp.Children.Add(App.CreateInputControl("Display name", this.sNameW, default));
        _ = this.stp.Children.Add(App.CreateOpenFileItem("Brand image", this.sImageW, "PNG Image|*.png|Other|*.*", new Thickness(0, 5, 0, 0)));
        _ = this.stp.Children.Add(App.CreateOpenFileItem("Computer icon", this.sIconComputerW, sIconFilter, new Thickness(0, 5, 0, 0)));
        _ = this.stp.Children.Add(App.CreateOpenFileItem("Documents icon", this.sIconDocumentsW, sIconFilter, new Thickness(0, 5, 0, 0)));
        _ = this.stp.Children.Add(App.CreateOpenFileItem("Network icon", this.sIconNetworkW, sIconFilter, new Thickness(0, 5, 0, 0)));
        _ = this.stp.Children.Add(App.CreateOpenFileItem("Recycle bin (full) icon", this.sIconRecycleBinFullW, sIconFilter, new Thickness(0, 5, 0, 0)));
        _ = this.stp.Children.Add(App.CreateOpenFileItem("Recycle bin (empty) icon", this.sIconRecycleBinEmptyW, sIconFilter, new Thickness(0, 5, 0, 0)));
        return;
    }
}
