namespace ThemeBuilder;

public interface IThemeBuilder
{
    string ThemeSection { get; }
    string BuildThemeSection();
}
