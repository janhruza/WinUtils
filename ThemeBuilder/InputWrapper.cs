namespace ThemeBuilder;

public class InputWrapper
{
    public InputWrapper()
    {
        Value = string.Empty;
    }

    public InputWrapper(string sValue)
    {
        Value = sValue;
    }

    public string Value { get; set; }
}
