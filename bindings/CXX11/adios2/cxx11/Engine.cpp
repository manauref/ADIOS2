/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * Engine.h :
 *
 *  Created on: Jun 4, 2018
 *      Author: William F Godoy godoywf@ornl.gov
 */

#include "Engine.h"
#include "Engine.tcc"

#include "adios2/core/Engine.h"

#include "adios2/helper/adiosFunctions.h"

namespace adios2
{

Engine::operator bool() const noexcept
{
    if (m_Engine == nullptr)
    {
        return false;
    }

    return *m_Engine ? true : false;
}

std::string Engine::Name() const
{
    helper::CheckForNullptr(m_Engine, "in call to Engine::Name");
    return m_Engine->m_Name;
}

std::string Engine::Type() const
{
    helper::CheckForNullptr(m_Engine, "in call to Engine::Type");
    return m_Engine->m_EngineType;
}

Mode Engine::OpenMode() const
{
    helper::CheckForNullptr(m_Engine, "in call to Engine::OpenMode");
    return m_Engine->OpenMode();
}

StepStatus Engine::BeginStep()
{
    helper::CheckForNullptr(m_Engine, "in call to Engine::BeginStep");
    return m_Engine->BeginStep();
}

StepStatus Engine::BeginStep(const StepMode mode, const float timeoutSeconds)
{
    helper::CheckForNullptr(
        m_Engine, "in call to Engine::BeginStep(const StepMode, const float)");
    return m_Engine->BeginStep(mode, timeoutSeconds);
}

size_t Engine::CurrentStep() const
{
    helper::CheckForNullptr(m_Engine, "in call to Engine::CurrentStep");
    return m_Engine->CurrentStep();
}

void Engine::PerformPuts()
{
    helper::CheckForNullptr(m_Engine, "in call to Engine::PerformPuts");
    m_Engine->PerformPuts();
}

void Engine::PerformDataWrite()
{
    helper::CheckForNullptr(m_Engine, "in call to Engine::PerformDataWrite");
    m_Engine->PerformDataWrite();
}

void Engine::PerformGets()
{
    helper::CheckForNullptr(m_Engine, "in call to Engine::PerformGets");
    m_Engine->PerformGets();
}

void Engine::LockWriterDefinitions()
{
    helper::CheckForNullptr(m_Engine,
                            "in call to Engine::LockWriterDefinitions");
    m_Engine->LockWriterDefinitions();
}

void Engine::LockReaderSelections()
{
    helper::CheckForNullptr(m_Engine,
                            "in call to Engine::LockReaderSelections");
    m_Engine->LockReaderSelections();
}

void Engine::EndStep()
{
    helper::CheckForNullptr(m_Engine, "in call to Engine::EndStep");
    m_Engine->EndStep();
}

void Engine::Flush(const int transportIndex)
{
    helper::CheckForNullptr(m_Engine, "in call to Engine::Flush");
    m_Engine->Flush(transportIndex);
}

void Engine::Close(const int transportIndex)
{
    helper::CheckForNullptr(m_Engine, "in call to Engine::Close");
    m_Engine->Close(transportIndex);

    // erase Engine object from IO
    core::IO &io = m_Engine->GetIO();
    const std::string name = m_Engine->m_Name;
    io.RemoveEngine(name);
    m_Engine = nullptr;
}

size_t Engine::Steps() const
{
    helper::CheckForNullptr(m_Engine, "in call to Engine::Steps");
    return m_Engine->Steps();
}

Engine::Engine(core::Engine *engine) : m_Engine(engine) {}

#define declare_template_instantiation(T)                                      \
                                                                               \
    template typename Variable<T>::Span Engine::Put(Variable<T>, const bool,   \
                                                    const T &);                \
    template typename Variable<T>::Span Engine::Put(Variable<T>);              \
    template void Engine::Get<T>(Variable<T>, T **) const;

ADIOS2_FOREACH_PRIMITIVE_TYPE_1ARG(declare_template_instantiation)
#undef declare_template_instantiation

#define declare_template_instantiation(T)                                      \
    template void Engine::Put<T>(Variable<T>, const T *, const Mode);          \
    template void Engine::Put<T>(const std::string &, const T *, const Mode);  \
    template void Engine::Put<T>(Variable<T>, const T &, const Mode);          \
    template void Engine::Put<T>(const std::string &, const T &, const Mode);  \
                                                                               \
    template void Engine::Get<T>(Variable<T>, T *, const Mode);                \
    template void Engine::Get<T>(const std::string &, T *, const Mode);        \
    template void Engine::Get<T>(Variable<T>, T &, const Mode);                \
    template void Engine::Get<T>(const std::string &, T &, const Mode);        \
                                                                               \
    template void Engine::Get<T>(Variable<T>, std::vector<T> &, const Mode);   \
    template void Engine::Get<T>(const std::string &, std::vector<T> &,        \
                                 const Mode);                                  \
                                                                               \
    template void Engine::Get<T>(                                              \
        Variable<T>, typename Variable<T>::Info & info, const Mode);           \
    template void Engine::Get<T>(                                              \
        const std::string &, typename Variable<T>::Info &info, const Mode);    \
                                                                               \
    template std::map<size_t, std::vector<typename Variable<T>::Info>>         \
    Engine::AllStepsBlocksInfo(const Variable<T> variable) const;              \
                                                                               \
    template std::vector<typename Variable<T>::Info> Engine::BlocksInfo(       \
        const Variable<T> variable, const size_t step) const;                  \
                                                                               \
    template std::vector<size_t> Engine::GetAbsoluteSteps(                     \
        const Variable<T> variable) const;

ADIOS2_FOREACH_TYPE_1ARG(declare_template_instantiation)
#undef declare_template_instantiation

size_t Engine::DebugGetDataBufferSize() const
{
    helper::CheckForNullptr(m_Engine,
                            "in call to Engine::DebugGetDataBufferSize");
    return m_Engine->DebugGetDataBufferSize();
}

std::string ToString(const Engine &engine)
{
    return std::string("Engine(Name: \"" + engine.Name() + "\", Type: \"" +
                       engine.Type() + "\")");
}

} // end namespace adios2
