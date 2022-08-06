#pragma once

template <class T>
T Clamp(T Current, T min, T max) {
	Current = (Current < min ? min : Current);
	Current = (Current > max ? max : Current);
	return Current;
}