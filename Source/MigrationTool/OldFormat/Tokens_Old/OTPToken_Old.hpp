#ifndef OTPTOKEN_OLD_HPP
#define OTPTOKEN_OLD_HPP

#include <string>
#include <cstdint>
#include <memory>

class OTPToken_Old
{
public:
    using TokenString = std::string;
    using SecretType = std::string;
    using Label = std::string;
    using Icon = std::string;
    using DigitType = std::uint8_t;
    using PeriodType = std::uint32_t;
    using CounterType = std::uint32_t;

    // Token limits
    static const DigitType min_digits;
    static const DigitType max_digits;
    static const PeriodType min_period;
    static const PeriodType max_period;
    static const CounterType min_counter;
    static const CounterType max_counter;

    OTPToken_Old();
    OTPToken_Old(const OTPToken_Old *other);
    OTPToken_Old(const Label &label);
    virtual ~OTPToken_Old();

    enum TokenType {
        None = 0,

        TOTP = 1,
        HOTP = 2,
        Steam = 3,
        Authy = 4, // TOTP w/ 7 digits and 10s period
    };

    enum ShaAlgorithm {
        Invalid = 0,

        SHA1    = 1,
        SHA256  = 2,
        SHA512  = 3,
    };

    // token type
    inline const TokenType &type() const
    { return _type; }
    // token name (type)
    inline const std::string &typeString() const
    { return _typeName; }

    // display name (label)
    inline void setLabel(const Label &label)
    { this->_label = label; }
    inline const Label &label() const
    { return _label; }

    // custom user icon (GUI only)
    inline void setIcon(const Icon &icon)
    { this->_icon = icon; }
    const Icon &icon() const
    { return _icon; }

    // base-32 encoded secret
    inline void setSecret(const SecretType &secret)
    { this->_secret = secret; }
    inline const SecretType &secret() const
    { return _secret; }

    // digit length
    inline void setDigitLength(const DigitType &digits)
    {
        switch (_type)
        {
            case TOTP:
            case HOTP:
            case Authy:
                this->_digits = digits;
                break;
        }
    }
    inline const DigitType &digits() const
    { return _digits; }

    // token validity period
    inline void setPeriod(const PeriodType &period)
    {
        switch (_type)
        {
            case TOTP:
            case Authy:
                this->_period = period;
                break;
        }
    }
    inline const PeriodType &period() const
    { return _period; }

    // token counter
    inline void setCounter(const CounterType &counter)
    {
        switch (_type)
        {
            case HOTP:
                this->_counter = counter;
                break;
        }
    }
    inline const CounterType &counter() const
    { return _counter; }

    // token algorithm
    inline void setAlgorithm(const ShaAlgorithm &algo)
    {
        switch (_type)
        {
            case TOTP:
            case HOTP:
                this->_algorithm = algo;
                break;
        }
    }
    void setAlgorithm(const std::string &algo);
    const ShaAlgorithm &algorithm() const
    { return _algorithm; }
    const std::string algorithmString() const;

#ifdef OTPGEN_DEBUG
    const std::string debug() const;
#endif

    // verify token validity
    bool valid() const;

    // calculate the remaining token validity from the period and current system time
    std::uint32_t remainingTokenValidity() const;

protected:
    TokenType _type = None;
    std::string _typeName = "None";

    Label _label = Label();
    Icon _icon = Icon();

    SecretType _secret = SecretType();
    DigitType _digits = 0U;
    PeriodType _period = 0U;
    CounterType _counter = 0U;
    ShaAlgorithm _algorithm = Invalid;

public:
    friend struct TokenData;
    friend class TokenStore_Old;

    virtual std::shared_ptr<OTPToken_Old> clone() const = 0;

    OTPToken_Old(
             const Label &label,
             const Icon &icon,
             const SecretType &secret,
             const DigitType &digits,
             const PeriodType &period,
             const CounterType &counter,
             const ShaAlgorithm &algorithm)
        : OTPToken_Old()
    {
        _label = label;
        _icon = icon;
        _secret = secret;
        this->setDigitLength(digits);
        this->setPeriod(period);
        this->setCounter(counter);
        this->setAlgorithm(algorithm);
    }
};

#endif // OTPTOKEN_OLD_HPP
