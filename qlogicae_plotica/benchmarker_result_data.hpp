#pragma once

#include "utilities.hpp"

#include "qlogicae_core.hpp"

namespace QLogicaePlotica
{
    class BenchmarkerResultData
    {
    public:
        BenchmarkerResultData(
            const size_t& input_size
        );
        
        ~BenchmarkerResultData() = default;
        
        BenchmarkerResultData(
            const BenchmarkerResultData&
        ) = delete;
        
        BenchmarkerResultData(
            BenchmarkerResultData&&
        ) noexcept = default;
        
        BenchmarkerResultData& operator=(
            const BenchmarkerResultData&
        ) = delete;
        
        BenchmarkerResultData& operator=(
            BenchmarkerResultData&&
        ) noexcept = default;

        std::any& get_payload();
        
        size_t get_input_size() const;
        
        void set_end_execution_time();
        
        void set_start_execution_time();
        
        double get_setup_duration_time() const;
        
        double get_execution_duration_time() const;
                
        const std::any& get_payload() const;
        
        void set_payload(
            const std::any& value
        );
        
        double get_end_execution_time() const;
        
        double get_start_execution_time() const;
        
        double get_end_setup_time() const;
        
        double get_start_setup_time() const;
        
        void set_end_setup_time();
        
        void set_start_setup_time();
        
        template <typename Type> Type& get_payload();
        
        template <typename Type> void set_payload(
            Type&& value
        );
        
        template <typename Type> const Type& get_payload() const;

    protected:
        std::any _payload;
        
        size_t _input_size;
        
        double _end_setup_time;
        
        double _start_setup_time;
        
        double _end_execution_time;
        
        double _start_execution_time;
    };

    template <typename Type>
    void BenchmarkerResultData::set_payload(
        Type&& value
    )
    {
        _payload = std::forward<Type>(value);
    }

    template <typename Type>
    Type& BenchmarkerResultData::get_payload()
    {
        return std::any_cast<Type&>(_payload);
    }

    template <typename Type>
    const Type& BenchmarkerResultData::get_payload() const
    {
        return std::any_cast<const Type&>(_payload);
    }
}
