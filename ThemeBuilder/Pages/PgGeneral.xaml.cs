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
        sb.AppendLine(ThemeSection);
        sb.AppendLine($"DisplayName={sName}");
        sb.AppendLine($"BrandImage={sImage}");
        sb.AppendLine($"[CLSID\\{{20D04FE0-3AEA-1069-A2D8-08002B30309D}}\\DefaultIcon]");
        sb.AppendLine($"DefaultValue={sIconComputer}");
        sb.AppendLine($"[CLSID\\{{59031A47-3F72-44A7-89C5-5595FE6B30EE}}\\DefaultIcon]");
        sb.AppendLine($"DefaultValue={sIconDocuments}");
        sb.AppendLine($"[CLSID\\{{F02C1A0D-BE21-4350-88B0-7367FC96EF3C}}\\DefaultIcon]");
        sb.AppendLine($"DefaultValue={sIconNetwork}");
        sb.AppendLine($"[CLSID\\{{645FF040-5081-101B-9F08-00AA002F954E}}\\DefaultIcon]");
        sb.AppendLine($"Full={sIconRecycleBinFull}");
        sb.AppendLine($"Empty={sIconRecycleBinEmpty}");
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
    public string sName => sNameW.Value;
    public string sImage => sImageW.Value;
    public string sIconComputer => sIconComputerW.Value;
    public string sIconDocuments => sIconDocumentsW.Value;
    public string sIconNetwork => sIconNetworkW.Value;
    public string sIconRecycleBinFull => sIconRecycleBinFullW.Value;
    public string sIconRecycleBinEmpty => sIconRecycleBinEmptyW.Value;

    private const string sIconFilter = "Icon Files|*.ico|Other|*.*";

    private void RefreshUI()
    {
        stp.Children.Clear();

        stp.Children.Add(App.CreateInputControl("Display name", sNameW, default));
        stp.Children.Add(App.CreateOpenFileItem("Brand image", sImageW, "PNG Image|*.png|Other|*.*", new Thickness(0, 5, 0, 0)));
        stp.Children.Add(App.CreateOpenFileItem("Computer icon", sIconComputerW, sIconFilter, new Thickness(0, 5, 0, 0)));
        stp.Children.Add(App.CreateOpenFileItem("Documents icon", sIconDocumentsW, sIconFilter, new Thickness(0, 5, 0, 0)));
        stp.Children.Add(App.CreateOpenFileItem("Network icon", sIconNetworkW, sIconFilter, new Thickness(0, 5, 0, 0)));
        stp.Children.Add(App.CreateOpenFileItem("Recycle bin (full) icon", sIconRecycleBinFullW, sIconFilter, new Thickness(0, 5, 0, 0)));
        stp.Children.Add(App.CreateOpenFileItem("Recycle bin (empty) icon", sIconRecycleBinEmptyW, sIconFilter, new Thickness(0, 5, 0, 0)));
        return;
    }
}
