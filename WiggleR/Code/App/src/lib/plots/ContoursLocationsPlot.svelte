<script lang="ts">
	import BasePlot from './BasePlot.svelte';
	import { start, end } from '../../store/store';
	import { getContoursData } from './data.svelte';

	function random_rgb() {
		var o = Math.round,
			r = Math.random,
			s = 255;
		return 'rgb(' + o(r() * s) + ',' + o(r() * s) + ',' + o(r() * s) + ')';
	}

	let startTime = '';
	let endTime = '';
	let trace: Plotly.Data;
	start.subscribe((time) => (startTime = time));
	end.subscribe((time) => (endTime = time));

	$: getTrace(startTime, endTime);

	async function getTrace(startTime: string, endTime: string) {
		const data = await getContoursData(startTime, endTime);

		trace = {
			x: data.contourLocations.flat().map(([x, _]: [Number, Number]) => x),
			y: data.contourLocations.flat().map(([_, y]: [Number, Number]) => y),
			type: 'scatter',
			mode: 'markers',
			marker: {
				color: data.contourLocations.map((frame: any) => { const rgb = random_rgb(); return frame.map(() => rgb) }).flat(),
				size: data.contourLocations.flat().map(([, , area]: [Number, Number, Number]) => area)
			}
		};
	}
</script>

{#if trace}
	<BasePlot {trace} title="Contour Locations" xaxisTitle="X" yaxisTitle="Y" />
{/if}
